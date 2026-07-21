#pragma once

#include <cstddef>
#include <cstdint>

namespace cattle_tracker {

constexpr std::uint16_t kProtocolMagic = 0xCA71;
constexpr std::uint8_t kProtocolVersion = 1;
constexpr std::size_t kEncodedLocationSize = 26;

enum LocationFlags : std::uint8_t {
  kFlagGnssFix = 1U << 0,
  kFlagGnssTimeValid = 1U << 1,
  kFlagBatteryValid = 1U << 2,
};

struct LocationData {
  std::uint16_t deviceId = 0;
  std::uint32_t sequence = 0;
  std::int32_t latitudeE7 = 0;
  std::int32_t longitudeE7 = 0;
  std::uint32_t gnssUnixTime = 0;
  std::uint16_t batteryMv = 0;
  std::uint8_t flags = 0;
};

inline std::uint16_t crc16Ccitt(const std::uint8_t* data, std::size_t length) {
  std::uint16_t crc = 0xFFFF;
  for (std::size_t i = 0; i < length; ++i) {
    crc ^= static_cast<std::uint16_t>(data[i]) << 8;
    for (std::uint8_t bit = 0; bit < 8; ++bit) {
      crc = (crc & 0x8000U) != 0
                ? static_cast<std::uint16_t>((crc << 1) ^ 0x1021U)
                : static_cast<std::uint16_t>(crc << 1);
    }
  }
  return crc;
}

inline void writeU16(std::uint8_t* output, std::uint16_t value) {
  output[0] = static_cast<std::uint8_t>(value);
  output[1] = static_cast<std::uint8_t>(value >> 8);
}

inline void writeU32(std::uint8_t* output, std::uint32_t value) {
  output[0] = static_cast<std::uint8_t>(value);
  output[1] = static_cast<std::uint8_t>(value >> 8);
  output[2] = static_cast<std::uint8_t>(value >> 16);
  output[3] = static_cast<std::uint8_t>(value >> 24);
}

inline std::uint16_t readU16(const std::uint8_t* input) {
  return static_cast<std::uint16_t>(input[0]) |
         (static_cast<std::uint16_t>(input[1]) << 8);
}

inline std::uint32_t readU32(const std::uint8_t* input) {
  return static_cast<std::uint32_t>(input[0]) |
         (static_cast<std::uint32_t>(input[1]) << 8) |
         (static_cast<std::uint32_t>(input[2]) << 16) |
         (static_cast<std::uint32_t>(input[3]) << 24);
}

inline bool encodeLocation(const LocationData& location, std::uint8_t* output,
                           std::size_t outputSize) {
  if (output == nullptr || outputSize < kEncodedLocationSize) {
    return false;
  }

  writeU16(output + 0, kProtocolMagic);
  output[2] = kProtocolVersion;
  writeU16(output + 3, location.deviceId);
  writeU32(output + 5, location.sequence);
  writeU32(output + 9, static_cast<std::uint32_t>(location.latitudeE7));
  writeU32(output + 13, static_cast<std::uint32_t>(location.longitudeE7));
  writeU32(output + 17, location.gnssUnixTime);
  writeU16(output + 21, location.batteryMv);
  output[23] = location.flags;
  writeU16(output + 24, crc16Ccitt(output, 24));
  return true;
}

inline bool decodeLocation(const std::uint8_t* input, std::size_t inputSize,
                           LocationData& location) {
  if (input == nullptr || inputSize != kEncodedLocationSize ||
      readU16(input + 0) != kProtocolMagic ||
      input[2] != kProtocolVersion ||
      readU16(input + 24) != crc16Ccitt(input, 24)) {
    return false;
  }

  location.deviceId = readU16(input + 3);
  location.sequence = readU32(input + 5);
  location.latitudeE7 = static_cast<std::int32_t>(readU32(input + 9));
  location.longitudeE7 = static_cast<std::int32_t>(readU32(input + 13));
  location.gnssUnixTime = readU32(input + 17);
  location.batteryMv = readU16(input + 21);
  location.flags = input[23];
  return true;
}

}  // namespace cattle_tracker
