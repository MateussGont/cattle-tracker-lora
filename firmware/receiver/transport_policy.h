#pragma once

#include <cstddef>
#include <cstdint>

namespace cattle_tracker {

constexpr std::int64_t kMinimumTlsUnixTime = 1767225600;  // 2026-01-01 UTC
constexpr std::uint32_t kClockWaitMs = 30000;
constexpr std::uint32_t kClockRetryMs = 60000;
constexpr std::size_t kMqttPacketCapacity = 1024;
constexpr std::size_t kTelemetryCapacity = 512;

enum class ClockAction { Waiting, StartSync, TimedOut, Ready };

// Pure state machine: no sleeps; safe across millis() wraparound.
class ClockSyncGate {
 public:
  ClockAction update(bool wifi, std::uint32_t now, std::int64_t unixTime, bool synchronized) {
    if (!wifi) {
      attempted_ = waiting_ = synchronized_ = ready_ = false;
      return ClockAction::Waiting;
    }
    if (synchronized) synchronized_ = true;
    ready_ = synchronized_ && unixTime >= kMinimumTlsUnixTime;
    if (ready_) return ClockAction::Ready;
    if (!attempted_ || static_cast<std::uint32_t>(now - startedAt_) >= kClockRetryMs) {
      attempted_ = waiting_ = true;
      startedAt_ = now;
      return ClockAction::StartSync;
    }
    if (waiting_ && static_cast<std::uint32_t>(now - startedAt_) >= kClockWaitMs) {
      waiting_ = false;
      return ClockAction::TimedOut;
    }
    return ClockAction::Waiting;
  }
  bool ready() const { return ready_; }

 private:
  bool attempted_ = false;
  bool waiting_ = false;
  bool synchronized_ = false;
  bool ready_ = false;
  std::uint32_t startedAt_ = 0;
};

inline bool mqttPayloadFits(std::size_t payloadSize, std::size_t topicSize) {
  // PubSubClient reserves a five-byte MQTT header and a two-byte topic length.
  return payloadSize < kTelemetryCapacity && topicSize <= 128 &&
         payloadSize + topicSize + 7 <= kMqttPacketCapacity;
}

}  // namespace cattle_tracker
