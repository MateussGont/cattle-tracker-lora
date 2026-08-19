#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h>

#include "mqtt_publisher.h"
#include "protocol.h"
#include "secrets.h"

namespace {

constexpr int kLoRaSck = 5;
constexpr int kLoRaMiso = 19;
constexpr int kLoRaMosi = 27;
constexpr int kLoRaNss = 18;
constexpr int kLoRaDio0 = 26;
constexpr int kLoRaReset = 14;
constexpr int kLoRaDio1 = 35;

// RadioLib's default blocking receive() times out after ~500% of the
// time-on-air of a max-length packet (well under 1 s at SF7/125 kHz), so an
// idle radio re-polls and logs "rx_error" several times per second. The
// collar only transmits every 10 s, so there is no reason to poll that
// often; widen the window each receive() call listens for before giving up.
constexpr uint32_t kReceiveTimeoutMs = 2000;

SX1276 radio = new Module(kLoRaNss, kLoRaDio0, kLoRaReset, kLoRaDio1);

const cattle_tracker::GatewayConfig kGatewayConfig{
    WIFI_SSID, WIFI_PASSWORD, MQTT_HOST,     MQTT_PORT,
    MQTT_USERNAME, MQTT_PASSWORD, MQTT_TOPIC, GATEWAY_ID,
};
cattle_tracker::MqttPublisher mqttPublisher(kGatewayConfig);

void stopWithRadioError(const char* operation, int16_t state) {
  Serial.printf("%s failed, RadioLib code %d\n", operation, state);
  while (true) {
    delay(1000);
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(1500);

  SPI.begin(kLoRaSck, kLoRaMiso, kLoRaMosi, kLoRaNss);
  const int16_t state = radio.begin(
      915.0, 125.0, 7, 5, RADIOLIB_SX127X_SYNC_WORD, 14, 8, 0);
  if (state != RADIOLIB_ERR_NONE) {
    stopWithRadioError("SX1276 init", state);
  }

  mqttPublisher.begin();

  Serial.println("receiver ready: Heltec WiFi LoRa 32 V2");
}

void loop() {
  // radio.receive() below blocks until a packet arrives or times out, so
  // Wi-Fi/MQTT housekeeping only runs in the gaps between packets. That is
  // fine at the collar's current 10 s transmit interval; a future multi-tag
  // gateway would need an interrupt/async receive to keep this responsive.
  mqttPublisher.loop();

  std::uint8_t payload[cattle_tracker::kEncodedLocationSize] = {};
  const int16_t state = radio.receive(payload, sizeof(payload), kReceiveTimeoutMs);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.printf("{\"event\":\"rx_error\",\"code\":%d}\n", state);
    return;
  }

  cattle_tracker::LocationData location;
  if (!cattle_tracker::decodeLocation(payload, sizeof(payload), location)) {
    Serial.println("{\"event\":\"invalid_payload\"}");
    return;
  }

  const float rssi = radio.getRSSI();
  const float snr = radio.getSNR();

  Serial.printf(
      "{\"device_id\":%u,\"sequence\":%lu,\"latitude\":%.7f,"
      "\"longitude\":%.7f,\"gnss_unix_time\":%lu,\"battery_mv\":%u,"
      "\"flags\":%u,\"rssi\":%.1f,\"snr\":%.1f}\n",
      location.deviceId, static_cast<unsigned long>(location.sequence),
      location.latitudeE7 / 10000000.0,
      location.longitudeE7 / 10000000.0,
      static_cast<unsigned long>(location.gnssUnixTime), location.batteryMv,
      location.flags, rssi, snr);

  if (!mqttPublisher.publishLocation(location, rssi, snr)) {
    Serial.println("{\"event\":\"mqtt_publish_skipped\"}");
  }
}
