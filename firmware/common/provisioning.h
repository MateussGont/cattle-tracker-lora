#pragma once

#include <cstdint>

#include "protocol.h"

namespace cattle_tracker {

constexpr std::uint16_t kProvisioningSchemaVersion = 1;
constexpr std::uint16_t kUnprovisionedRadioDeviceId = 0;

struct DeviceConfig {
  std::uint16_t schemaVersion = kProvisioningSchemaVersion;
  std::uint16_t radioDeviceId = kUnprovisionedRadioDeviceId;
  std::uint32_t configRevision = 0;
  std::uint16_t checksum = 0;
};

inline std::uint16_t deviceConfigChecksum(const DeviceConfig& config) {
  std::uint8_t bytes[8] = {};
  writeU16(bytes, config.schemaVersion);
  writeU16(bytes + 2, config.radioDeviceId);
  writeU32(bytes + 4, config.configRevision);
  return crc16Ccitt(bytes, sizeof(bytes));
}

inline DeviceConfig makeDeviceConfig(std::uint16_t radioDeviceId,
                                     std::uint32_t configRevision) {
  DeviceConfig config;
  config.radioDeviceId = radioDeviceId;
  config.configRevision = configRevision;
  config.checksum = deviceConfigChecksum(config);
  return config;
}

inline bool isValidDeviceConfig(const DeviceConfig& config) {
  return config.schemaVersion == kProvisioningSchemaVersion &&
         config.radioDeviceId != kUnprovisionedRadioDeviceId &&
         config.configRevision > 0 &&
         config.checksum == deviceConfigChecksum(config);
}

enum class ProvisionDecision {
  kApply,
  kIdempotent,
  kInvalidRadioDeviceId,
  kInvalidRevision,
  kAlreadyProvisioned,
};

inline ProvisionDecision decideProvisioning(const DeviceConfig* current,
                                             std::uint16_t radioDeviceId,
                                             std::uint32_t configRevision) {
  if (radioDeviceId == kUnprovisionedRadioDeviceId) {
    return ProvisionDecision::kInvalidRadioDeviceId;
  }
  if (configRevision == 0) {
    return ProvisionDecision::kInvalidRevision;
  }
  if (current == nullptr || !isValidDeviceConfig(*current)) {
    return ProvisionDecision::kApply;
  }
  if (current->radioDeviceId == radioDeviceId &&
      current->configRevision == configRevision) {
    return ProvisionDecision::kIdempotent;
  }
  return ProvisionDecision::kAlreadyProvisioned;
}

}  // namespace cattle_tracker
