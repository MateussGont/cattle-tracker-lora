#include "mqtt_publisher.h"

#include <ArduinoJson.h>
#include <esp_sntp.h>
#include <cstring>
#include <ctime>

#include "mqtt_root_ca.h"

namespace cattle_tracker {

namespace {
constexpr unsigned long kWifiRetryIntervalMs = 5000;
constexpr unsigned long kMqttRetryIntervalMs = 5000;
}  // namespace

MqttPublisher::MqttPublisher(const GatewayConfig& config)
    : config_(config), mqttClient_(wifiClient_) {}

void MqttPublisher::begin() {
  IPAddress numericHost;
  if (!config_.mqttHost || !config_.mqttHost[0] ||
      numericHost.fromString(config_.mqttHost) || config_.mqttPort != 8883 ||
      !config_.mqttTopic || !config_.mqttTopic[0] || std::strlen(config_.mqttTopic) > 128 ||
      !config_.mqttUsername || !config_.mqttUsername[0] ||
      !config_.mqttPassword || !config_.mqttPassword[0] ||
      !config_.gatewayId || !config_.gatewayId[0]) {
    Serial.println("mqtt TLS configuration invalid: use DNS host, port 8883 and credentials");
    return;
  }
  if (!mqttClient_.setBufferSize(kMqttPacketCapacity)) {
    Serial.println("mqtt buffer allocation failed");
    return;
  }
  wifiClient_.setCACert(kMqttRootCa);
  wifiClient_.setHandshakeTimeout(8);  // Arduino-ESP32: seconds.
  wifiClient_.setTimeout(5);           // WiFiClientSecure: seconds, including TCP timeout.
  mqttClient_.setSocketTimeout(5);
  mqttClient_.setKeepAlive(30);
  WiFi.mode(WIFI_STA);
  mqttClient_.setServer(config_.mqttHost, config_.mqttPort);
  enabled_ = true;
  ensureWifi();
}

void MqttPublisher::loop() {
  if (!enabled_) return;
  ensureWifi();
  const bool wifiConnected = WiFi.status() == WL_CONNECTED;
  const auto clockAction = clockGate_.update(
      wifiConnected, millis(), static_cast<std::int64_t>(std::time(nullptr)),
      sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED);
  if (clockAction == ClockAction::StartSync) {
    sntp_set_sync_status(SNTP_SYNC_STATUS_RESET);
    configTime(0, 0, "time.cloudflare.com", "pool.ntp.org", "time.google.com");
    Serial.println("waiting for SNTP before MQTT TLS");
  } else if (clockAction == ClockAction::TimedOut) {
    Serial.println("SNTP timeout; MQTT remains disabled until clock sync");
  }
  if (clockAction == ClockAction::Ready) {
    ensureMqtt();
    if (mqttClient_.connected()) {
      mqttClient_.loop();
    }
  } else {
    if (mqttClient_.connected()) mqttClient_.disconnect();
    wifiClient_.stop();
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

  Serial.println("connecting to wifi");
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
  Serial.println("connecting to MQTT over verified TLS");
  const bool connected = mqttClient_.connect(config_.gatewayId, config_.mqttUsername, config_.mqttPassword);
  // Backoff starts after the synchronous connection attempt completes.
  lastMqttAttemptMs_ = millis();
  if (!connected) {
    Serial.printf("mqtt connect failed, state=%d\n", mqttClient_.state());
    wifiClient_.stop();
  }
}

bool MqttPublisher::publishLocation(const LocationData& location, float rssi, float snr) {
  if (!enabled_ || !clockGate_.ready() || !mqttClient_.connected()) {
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

  const size_t required = measureJson(doc);
  if (!mqttPayloadFits(required, std::strlen(config_.mqttTopic))) {
    Serial.println("mqtt payload rejected: packet capacity exceeded");
    return false;
  }
  char payload[kTelemetryCapacity];
  const size_t length = serializeJson(doc, payload, sizeof(payload));
  if (length != required) return false;
  return mqttClient_.publish(config_.mqttTopic, reinterpret_cast<const uint8_t*>(payload), length, false);
}

}  // namespace cattle_tracker
