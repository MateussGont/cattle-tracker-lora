#include <cassert>
#include <cstdint>
#include <iostream>

#include "../firmware/common/protocol.h"

int main() {
  cattle_tracker::LocationData expected;
  expected.deviceId = 27;
  expected.sequence = 123456;
  expected.latitudeE7 = -199231234;
  expected.longitudeE7 = -439401234;
  expected.gnssUnixTime = 1784635200;
  expected.batteryMv = 3890;
  expected.flags = cattle_tracker::kFlagGnssFix |
                   cattle_tracker::kFlagGnssTimeValid |
                   cattle_tracker::kFlagBatteryValid;

  std::uint8_t encoded[cattle_tracker::kEncodedLocationSize] = {};
  assert(cattle_tracker::encodeLocation(expected, encoded, sizeof(encoded)));

  cattle_tracker::LocationData decoded;
  assert(cattle_tracker::decodeLocation(encoded, sizeof(encoded), decoded));
  assert(decoded.deviceId == expected.deviceId);
  assert(decoded.sequence == expected.sequence);
  assert(decoded.latitudeE7 == expected.latitudeE7);
  assert(decoded.longitudeE7 == expected.longitudeE7);
  assert(decoded.gnssUnixTime == expected.gnssUnixTime);
  assert(decoded.batteryMv == expected.batteryMv);
  assert(decoded.flags == expected.flags);

  encoded[9] ^= 0x01;
  assert(!cattle_tracker::decodeLocation(encoded, sizeof(encoded), decoded));

  std::cout << "protocol tests passed\n";
  return 0;
}
