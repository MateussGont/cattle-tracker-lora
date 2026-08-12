#include "mqtt_publisher.h"

#include <ArduinoJson.h>

namespace cattle_tracker {

namespace {
constexpr unsigned long kWifiRetryIntervalMs = 5000;
constexpr unsigned long kMqttRetryIntervalMs = 5000;
}  // namespace

MqttPublisher::MqttPublisher(const GatewayConfig& config)
    : config_(config), mqttClient_(wifiClient_) {}

void MqttPublisher::begin() {
  WiFi.mode(WIFI_STA);
  mqttClient_.setServer(config_.mqttHost, config_.mqttPort);
  ensureWifi();
}

void MqttPublisher::loop() {
  ensureWifi();
  if (WiFi.status() == WL_CONNECTED) {
    ensureMqtt();
    if (mqttClient_.connected()) {
      mqttClient_.loop();
    }
  }
}

void MqttPublisher::ensureWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastWifiAttemptMs_ < kWifiRetryIntervalMs) {
    return;
  }
  lastWifiAttemptMs_ = now;

  Serial.printf("connecting to wifi \"%s\"\n", config_.wifiSsid);
  WiFi.begin(config_.wifiSsid, config_.wifiPassword);
}

void MqttPublisher::ensureMqtt() {
  if (mqttClient_.connected()) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastMqttAttemptMs_ < kMqttRetryIntervalMs) {
    return;
  }
  lastMqttAttemptMs_ = now;

  Serial.printf("connecting to mqtt broker %s:%u\n", config_.mqttHost, config_.mqttPort);
  const bool connected = mqttClient_.connect(config_.gatewayId, config_.mqttUsername, config_.mqttPassword);
  if (!connected) {
    Serial.printf("mqtt connect failed, state=%d\n", mqttClient_.state());
  }
}

bool MqttPublisher::publishLocation(const LocationData& location, float rssi, float snr) {
  if (!mqttClient_.connected()) {
    return false;
  }

  JsonDocument doc;
  doc["gatewayId"] = config_.gatewayId;
  doc["radioDeviceId"] = location.deviceId;
  doc["sequence"] = location.sequence;
  doc["latitude"] = location.latitudeE7 / 10000000.0;
  doc["longitude"] = location.longitudeE7 / 10000000.0;
  doc["gnssUnixTime"] = location.gnssUnixTime;
  doc["batteryMv"] = location.batteryMv;
  doc["flags"] = location.flags;
  doc["rssi"] = rssi;
  doc["snr"] = snr;

  char payload[256];
  const size_t length = serializeJson(doc, payload, sizeof(payload));
  return mqttClient_.publish(config_.mqttTopic, reinterpret_cast<const uint8_t*>(payload), length, false);
}

}  // namespace cattle_tracker
