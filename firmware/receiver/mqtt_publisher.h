#pragma once

#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "protocol.h"
#include "transport_policy.h"

namespace cattle_tracker {

struct GatewayConfig {
  const char* wifiSsid;
  const char* wifiPassword;
  const char* mqttHost;
  uint16_t mqttPort;
  const char* mqttUsername;
  const char* mqttPassword;
  const char* mqttTopic;
  const char* gatewayId;
};

/**
 * Owns the Wi-Fi + MQTT link and publishes decoded telemetry as JSON
 * matching backend/src/schemas/telemetry.ts. Kept isolated from main.cpp's
 * LoRa receive/decode loop so the gateway transport (MQTT today) can be
 * swapped later — e.g. for a LoRaWAN network server webhook — without
 * touching the radio code.
 *
 * SNTP waiting and retry scheduling do not block. DNS/TCP/TLS/MQTT calls
 * are still synchronous; timeouts are bounded, but LoRa may miss packets
 * during connection attempts. The async radio pipeline remains separate work.
 */
class MqttPublisher {
 public:
  explicit MqttPublisher(const GatewayConfig& config);

  void begin();

  /** Call once per main loop() iteration; reconnects as needed. */
  void loop();

  bool publishLocation(const LocationData& location, float rssi, float snr);

 private:
  GatewayConfig config_;
  WiFiClientSecure wifiClient_;
  PubSubClient mqttClient_;
  ClockSyncGate clockGate_;
  bool enabled_ = false;
  unsigned long lastWifiAttemptMs_ = 0;
  unsigned long lastMqttAttemptMs_ = 0;

  void ensureWifi();
  void ensureMqtt();
};

}  // namespace cattle_tracker
