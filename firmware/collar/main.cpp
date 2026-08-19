#include <Arduino.h>
#include <Preferences.h>
#include <RadioLib.h>
#include <SPI.h>
#include <TinyGPSPlus.h>

#include "protocol.h"

namespace {

constexpr unsigned long kSendIntervalMs = 10000;
constexpr unsigned long kGnssReadWindowMs = 2500;

constexpr int kLoRaSck = 7;
constexpr int kLoRaMiso = 8;
constexpr int kLoRaMosi = 9;
constexpr int kLoRaNss = 41;
constexpr int kLoRaDio1 = 39;
constexpr int kLoRaReset = 42;
constexpr int kLoRaBusy = 40;
constexpr int kLoRaRfSwitch = 38;

constexpr int kGnssRx = 43;  // XIAO D7, connect to GNSS TX.
constexpr int kGnssTx = 44;  // XIAO D6, connect to GNSS RX (optional).
constexpr std::uint32_t kGnssBaud = 9600;

// Identity (radioDeviceId) is provisioned at first-use over USB serial, not
// baked into the firmware image at compile time. This lets a single generic
// binary be flashed once per hardware batch, with each physical unit
// getting its number assigned later through the app's provisioning wizard
// (see docs/architecture.md, "Provisionamento de dispositivos"). See
// tryHandleSerialCommand() below for the wire protocol.
constexpr const char* kPrefsNamespace = "cattle";
constexpr const char* kPrefsKeyRadioId = "radioId";
constexpr unsigned long kUnprovisionedNoticeIntervalMs = 2000;

SX1262 radio = new Module(kLoRaNss, kLoRaDio1, kLoRaReset, kLoRaBusy);
TinyGPSPlus gps;
HardwareSerial gnssSerial(1);
Preferences preferences;
std::uint32_t sequenceNumber = 0;

std::uint16_t deviceId = 0;
bool provisioned = false;
unsigned long lastUnprovisionedNoticeMs = 0;

std::int64_t daysFromCivil(int year, unsigned month, unsigned day) {
  year -= month <= 2;
  const int era = (year >= 0 ? year : year - 399) / 400;
  const unsigned yearOfEra = static_cast<unsigned>(year - era * 400);
  const int adjustedMonth = static_cast<int>(month) + (month > 2 ? -3 : 9);
  const unsigned dayOfYear =
      (153U * static_cast<unsigned>(adjustedMonth) + 2U) / 5U + day - 1U;
  const unsigned dayOfEra =
      yearOfEra * 365U + yearOfEra / 4U - yearOfEra / 100U + dayOfYear;
  return static_cast<std::int64_t>(era) * 146097 + dayOfEra - 719468;
}

std::uint32_t gnssUnixTime() {
  if (!gps.date.isValid() || !gps.time.isValid()) {
    return 0;
  }

  const std::int64_t days =
      daysFromCivil(gps.date.year(), gps.date.month(), gps.date.day());
  const std::int64_t seconds =
      days * 86400 + gps.time.hour() * 3600 + gps.time.minute() * 60 +
      gps.time.second();
  return seconds > 0 ? static_cast<std::uint32_t>(seconds) : 0;
}

cattle_tracker::LocationData readLocation() {
  const unsigned long startedAt = millis();
  while (millis() - startedAt < kGnssReadWindowMs) {
    while (gnssSerial.available() > 0) {
      gps.encode(static_cast<char>(gnssSerial.read()));
    }
    delay(2);
  }

  cattle_tracker::LocationData location;
  location.deviceId = deviceId;
  location.sequence = ++sequenceNumber;

  if (gps.location.isValid() && gps.location.age() < 5000) {
    location.latitudeE7 =
        static_cast<std::int32_t>(gps.location.lat() * 10000000.0);
    location.longitudeE7 =
        static_cast<std::int32_t>(gps.location.lng() * 10000000.0);
    location.flags |= cattle_tracker::kFlagGnssFix;
  }

  location.gnssUnixTime = gnssUnixTime();
  if (location.gnssUnixTime != 0) {
    location.flags |= cattle_tracker::kFlagGnssTimeValid;
  }

  // The XIAO ESP32-S3 does not expose battery measurement by default.
  location.batteryMv = 0;
  return location;
}

void stopWithRadioError(const char* operation, int16_t state) {
  Serial.printf("%s failed, RadioLib code %d\n", operation, state);
  while (true) {
    delay(1000);
  }
}

/**
 * Text line protocol over USB serial (115200 8N1), used by the app's
 * device-provisioning wizard:
 *   "SET_RADIO_ID <0-65535>"  -> stores the id in NVS, replies
 *                                 {"event":"provisioned","radioDeviceId":N}
 *   "GET_STATUS"              -> replies {"event":"status", ...} without
 *                                 changing anything, so the app can check
 *                                 what a plugged-in unit is already set to.
 * Accepted at any time (not just first boot), so re-assigning a collar to a
 * different animal later is just sending SET_RADIO_ID again — no reflash.
 */
bool tryHandleSerialCommand() {
  if (!Serial.available()) {
    return false;
  }

  String line = Serial.readStringUntil('\n');
  line.trim();
  if (line.length() == 0) {
    return false;
  }

  if (line == "GET_STATUS") {
    Serial.printf(
        "{\"event\":\"status\",\"provisioned\":%s,\"radioDeviceId\":%u}\n",
        provisioned ? "true" : "false", deviceId);
    return true;
  }

  unsigned int parsedId = 0;
  if (sscanf(line.c_str(), "SET_RADIO_ID %u", &parsedId) == 1 && parsedId <= 0xFFFF) {
    deviceId = static_cast<std::uint16_t>(parsedId);
    preferences.putUShort(kPrefsKeyRadioId, deviceId);
    provisioned = true;
    Serial.printf("{\"event\":\"provisioned\",\"radioDeviceId\":%u}\n", deviceId);
    return true;
  }

  Serial.println(
      "{\"event\":\"provision_error\",\"message\":\"expected 'SET_RADIO_ID <0-65535>' or 'GET_STATUS'\"}");
  return false;
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(1500);

  preferences.begin(kPrefsNamespace, false);
  provisioned = preferences.isKey(kPrefsKeyRadioId);
  if (provisioned) {
    deviceId = preferences.getUShort(kPrefsKeyRadioId, 0);
  }

  gnssSerial.begin(kGnssBaud, SERIAL_8N1, kGnssRx, kGnssTx);
  SPI.begin(kLoRaSck, kLoRaMiso, kLoRaMosi, kLoRaNss);

  pinMode(kLoRaRfSwitch, OUTPUT);
  digitalWrite(kLoRaRfSwitch, HIGH);  // Receive/idle path on Wio-SX1262.

  const int16_t state = radio.begin(
      915.0, 125.0, 7, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 14, 8, 1.8,
      false);
  if (state != RADIOLIB_ERR_NONE) {
    stopWithRadioError("SX1262 init", state);
  }

  Serial.println("collar ready: XIAO ESP32-S3 + Wio-SX1262");
  Serial.printf(
      "{\"event\":\"boot\",\"provisioned\":%s,\"radioDeviceId\":%u}\n",
      provisioned ? "true" : "false", deviceId);
}

void loop() {
  tryHandleSerialCommand();

  if (!provisioned) {
    // Nothing to transmit yet: no radioDeviceId has been assigned to this
    // physical unit. Idle and keep announcing readiness so the app's
    // provisioning wizard can confirm it is talking to the right port.
    const unsigned long now = millis();
    if (now - lastUnprovisionedNoticeMs >= kUnprovisionedNoticeIntervalMs) {
      lastUnprovisionedNoticeMs = now;
      Serial.println("{\"event\":\"awaiting_provisioning\"}");
    }
    delay(50);
    return;
  }

  const cattle_tracker::LocationData location = readLocation();
  std::uint8_t payload[cattle_tracker::kEncodedLocationSize] = {};
  if (!cattle_tracker::encodeLocation(location, payload, sizeof(payload))) {
    Serial.println("payload encoding failed");
    delay(kSendIntervalMs);
    return;
  }

  digitalWrite(kLoRaRfSwitch, LOW);  // Transmit path on Wio-SX1262.
  const int16_t state = radio.transmit(payload, sizeof(payload));
  digitalWrite(kLoRaRfSwitch, HIGH);

  Serial.printf(
      "seq=%lu fix=%u lat=%.7f lon=%.7f tx_state=%d\n",
      static_cast<unsigned long>(location.sequence),
      (location.flags & cattle_tracker::kFlagGnssFix) != 0,
      location.latitudeE7 / 10000000.0,
      location.longitudeE7 / 10000000.0, state);

  delay(kSendIntervalMs);
}
