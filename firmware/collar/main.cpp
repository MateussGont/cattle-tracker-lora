#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <RadioLib.h>
#include <SPI.h>
#include <TinyGPSPlus.h>

#include "protocol.h"
#include "provisioning.h"

namespace {

constexpr char kFirmwareVersion[] = "0.2.0";
constexpr unsigned long kSendIntervalMs = 10000;
constexpr unsigned long kGnssWarmupMs = 2500;
constexpr unsigned long kUnprovisionedNoticeIntervalMs = 2000;
constexpr std::size_t kSerialLineCapacity = 384;

constexpr int kLoRaSck = 7;
constexpr int kLoRaMiso = 8;
constexpr int kLoRaMosi = 9;
constexpr int kLoRaNss = 41;
constexpr int kLoRaDio1 = 39;
constexpr int kLoRaReset = 42;
constexpr int kLoRaBusy = 40;
constexpr int kLoRaRfSwitch = 38;

constexpr int kGnssRx = 43;
constexpr int kGnssTx = 44;
constexpr std::uint32_t kGnssBaud = 9600;

constexpr char kPrefsNamespace[] = "cattle";
constexpr char kPrefsKeyDeviceConfig[] = "deviceConfig";

SX1262 radio = new Module(kLoRaNss, kLoRaDio1, kLoRaReset, kLoRaBusy);
TinyGPSPlus gps;
HardwareSerial gnssSerial(1);
Preferences preferences;
cattle_tracker::DeviceConfig deviceConfig;

std::uint32_t sequenceNumber = 0;
bool provisioned = false;
bool radioReady = false;
unsigned long bootMs = 0;
unsigned long lastSendMs = 0;
unsigned long lastUnprovisionedNoticeMs = 0;
char serialLine[kSerialLineCapacity] = {};
std::size_t serialLineLength = 0;

String hardwareUid() {
  const std::uint64_t efuseMac = ESP.getEfuseMac();
  char value[13] = {};
  snprintf(value, sizeof(value), "%04X%08X",
           static_cast<unsigned int>((efuseMac >> 32) & 0xFFFFU),
           static_cast<unsigned int>(efuseMac & 0xFFFFFFFFU));
  return String(value);
}

bool isValidRequestId(const char* requestId) {
  if (requestId == nullptr) {
    return false;
  }
  const std::size_t length = strlen(requestId);
  if (length == 0 || length > 64) {
    return false;
  }
  for (std::size_t index = 0; index < length; ++index) {
    const char value = requestId[index];
    if (!isalnum(static_cast<unsigned char>(value)) && value != '-' &&
        value != '_') {
      return false;
    }
  }
  return true;
}

void writeDeviceInfo(const char* requestId, const char* event = "device_info") {
  JsonDocument response;
  response["event"] = event;
  if (requestId != nullptr) {
    response["requestId"] = requestId;
  }
  response["hardwareUid"] = hardwareUid();
  response["firmwareVersion"] = kFirmwareVersion;
  response["provisioned"] = provisioned;
  response["radioDeviceId"] =
      provisioned ? deviceConfig.radioDeviceId
                  : cattle_tracker::kUnprovisionedRadioDeviceId;
  response["configRevision"] = provisioned ? deviceConfig.configRevision : 0;
  response["radioReady"] = radioReady;
  serializeJson(response, Serial);
  Serial.println();
}

void writeProvisionError(const char* requestId, const char* code,
                         const char* message) {
  JsonDocument response;
  response["event"] = "provision_error";
  if (requestId != nullptr) {
    response["requestId"] = requestId;
  }
  response["code"] = code;
  response["message"] = message;
  serializeJson(response, Serial);
  Serial.println();
}

bool persistDeviceConfig(const cattle_tracker::DeviceConfig& requested) {
  const std::size_t written = preferences.putBytes(
      kPrefsKeyDeviceConfig, &requested, sizeof(requested));
  if (written != sizeof(requested)) {
    return false;
  }

  cattle_tracker::DeviceConfig stored;
  const std::size_t read = preferences.getBytes(
      kPrefsKeyDeviceConfig, &stored, sizeof(stored));
  if (read != sizeof(stored) ||
      !cattle_tracker::isValidDeviceConfig(stored) ||
      stored.radioDeviceId != requested.radioDeviceId ||
      stored.configRevision != requested.configRevision) {
    return false;
  }

  deviceConfig = stored;
  provisioned = true;
  return true;
}

void handleProvision(JsonDocument& request, const char* requestId) {
  const char* requestedHardwareUid = request["hardwareUid"] | "";
  if (!hardwareUid().equalsIgnoreCase(requestedHardwareUid)) {
    writeProvisionError(requestId, "hardware_uid_mismatch",
                        "hardwareUid does not match this device");
    return;
  }
  if (!request["radioDeviceId"].is<unsigned int>() ||
      !request["configRevision"].is<unsigned long>()) {
    writeProvisionError(requestId, "invalid_payload",
                        "radioDeviceId and configRevision are required");
    return;
  }

  const unsigned int requestedRadioId = request["radioDeviceId"];
  const unsigned long requestedRevision = request["configRevision"];
  if (requestedRadioId > 0xFFFFU) {
    writeProvisionError(requestId, "invalid_radio_device_id",
                        "radioDeviceId must be between 1 and 65535");
    return;
  }

  const auto decision = cattle_tracker::decideProvisioning(
      provisioned ? &deviceConfig : nullptr,
      static_cast<std::uint16_t>(requestedRadioId),
      static_cast<std::uint32_t>(requestedRevision));
  if (decision == cattle_tracker::ProvisionDecision::kIdempotent) {
    writeDeviceInfo(requestId, "provision_result");
    return;
  }
  if (decision == cattle_tracker::ProvisionDecision::kInvalidRadioDeviceId) {
    writeProvisionError(requestId, "invalid_radio_device_id",
                        "radioDeviceId zero is reserved");
    return;
  }
  if (decision == cattle_tracker::ProvisionDecision::kInvalidRevision) {
    writeProvisionError(requestId, "invalid_config_revision",
                        "configRevision must be positive");
    return;
  }
  if (decision == cattle_tracker::ProvisionDecision::kAlreadyProvisioned) {
    writeProvisionError(requestId, "already_provisioned",
                        "device identity changes require an authorized recovery flow");
    return;
  }

  const auto requestedConfig = cattle_tracker::makeDeviceConfig(
      static_cast<std::uint16_t>(requestedRadioId),
      static_cast<std::uint32_t>(requestedRevision));
  if (!persistDeviceConfig(requestedConfig)) {
    writeProvisionError(requestId, "persistence_failed",
                        "configuration could not be verified in NVS");
    return;
  }
  writeDeviceInfo(requestId, "provision_result");
}

void handleSerialLine(char* line) {
  while (*line == ' ' || *line == '\t' || *line == '\r') {
    ++line;
  }
  if (*line == '\0') {
    return;
  }

  if (strcmp(line, "GET_STATUS") == 0) {
    writeDeviceInfo(nullptr, "status");
    return;
  }
  if (strncmp(line, "SET_RADIO_ID", 12) == 0) {
    writeProvisionError(nullptr, "legacy_write_disabled",
                        "use the correlated JSON provision command");
    return;
  }

  JsonDocument request;
  const DeserializationError error = deserializeJson(request, line);
  if (error || !request.is<JsonObject>()) {
    writeProvisionError(nullptr, "invalid_json", "expected one JSON object per line");
    return;
  }

  const char* requestId = request["requestId"] | nullptr;
  if (!isValidRequestId(requestId)) {
    writeProvisionError(nullptr, "invalid_request_id",
                        "requestId must be 1-64 URL-safe characters");
    return;
  }
  const char* command = request["cmd"] | "";
  if (strcmp(command, "get_info") == 0) {
    writeDeviceInfo(requestId);
    return;
  }
  if (strcmp(command, "provision") == 0) {
    handleProvision(request, requestId);
    return;
  }
  writeProvisionError(requestId, "unknown_command", "unsupported command");
}

void pollSerial() {
  while (Serial.available() > 0) {
    const char value = static_cast<char>(Serial.read());
    if (value == '\n') {
      serialLine[serialLineLength] = '\0';
      handleSerialLine(serialLine);
      serialLineLength = 0;
      continue;
    }
    if (value == '\r') {
      continue;
    }
    if (serialLineLength + 1 >= kSerialLineCapacity) {
      serialLineLength = 0;
      writeProvisionError(nullptr, "line_too_long", "serial command exceeded 383 bytes");
      continue;
    }
    serialLine[serialLineLength++] = value;
  }
}

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

void pollGnss() {
  while (gnssSerial.available() > 0) {
    gps.encode(static_cast<char>(gnssSerial.read()));
  }
}

cattle_tracker::LocationData currentLocation() {
  cattle_tracker::LocationData location;
  location.deviceId = deviceConfig.radioDeviceId;
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
  return location;
}

void transmitLocation() {
  const cattle_tracker::LocationData location = currentLocation();
  std::uint8_t payload[cattle_tracker::kEncodedLocationSize] = {};
  if (!cattle_tracker::encodeLocation(location, payload, sizeof(payload))) {
    Serial.println("payload encoding failed");
    return;
  }

  digitalWrite(kLoRaRfSwitch, LOW);
  const int16_t state = radio.transmit(payload, sizeof(payload));
  digitalWrite(kLoRaRfSwitch, HIGH);
  Serial.printf("seq=%lu fix=%u lat=%.7f lon=%.7f tx_state=%d\n",
                static_cast<unsigned long>(location.sequence),
                (location.flags & cattle_tracker::kFlagGnssFix) != 0,
                location.latitudeE7 / 10000000.0,
                location.longitudeE7 / 10000000.0, state);
}

}  // namespace

void setup() {
  Serial.begin(115200);
  bootMs = millis();

  preferences.begin(kPrefsNamespace, false);
  if (preferences.getBytesLength(kPrefsKeyDeviceConfig) == sizeof(deviceConfig) &&
      preferences.getBytes(kPrefsKeyDeviceConfig, &deviceConfig,
                           sizeof(deviceConfig)) == sizeof(deviceConfig)) {
    provisioned = cattle_tracker::isValidDeviceConfig(deviceConfig);
  }

  gnssSerial.begin(kGnssBaud, SERIAL_8N1, kGnssRx, kGnssTx);
  SPI.begin(kLoRaSck, kLoRaMiso, kLoRaMosi, kLoRaNss);
  pinMode(kLoRaRfSwitch, OUTPUT);
  digitalWrite(kLoRaRfSwitch, HIGH);

  const int16_t state = radio.begin(
      915.0, 125.0, 7, 5, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 14, 8, 1.8,
      false);
  radioReady = state == RADIOLIB_ERR_NONE;
  if (!radioReady) {
    Serial.printf("SX1262 init failed, RadioLib code %d\n", state);
  }
  writeDeviceInfo(nullptr, "boot");
}

void loop() {
  pollSerial();
  pollGnss();

  const unsigned long now = millis();
  if (!provisioned) {
    if (now - lastUnprovisionedNoticeMs >= kUnprovisionedNoticeIntervalMs) {
      lastUnprovisionedNoticeMs = now;
      Serial.println("{\"event\":\"awaiting_provisioning\"}");
    }
    delay(2);
    return;
  }

  if (radioReady && now - bootMs >= kGnssWarmupMs &&
      now - lastSendMs >= kSendIntervalMs) {
    lastSendMs = now;
    transmitLocation();
  }
  delay(2);
}
