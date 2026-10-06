#include <cassert>
#include <cstdint>
#include <iostream>
#include "../firmware/receiver/transport_policy.h"

int main() {
  using namespace cattle_tracker;
  ClockSyncGate clock;
  const auto valid = kMinimumTlsUnixTime + 86400;
  assert(clock.update(false, 0, valid, true) == ClockAction::Waiting);
  assert(!clock.ready());
  // A plausible date alone is not a completed synchronization.
  assert(clock.update(true, 0, valid, false) == ClockAction::StartSync);
  assert(clock.update(true, 29999, valid, false) == ClockAction::Waiting);
  assert(clock.update(true, 30000, valid, false) == ClockAction::TimedOut);
  assert(clock.update(true, 30001, valid, false) == ClockAction::Waiting);
  assert(clock.update(true, 59999, valid, false) == ClockAction::Waiting);
  assert(clock.update(true, 60000, valid, false) == ClockAction::StartSync);
  assert(clock.update(true, 60001, 0, true) == ClockAction::Waiting);
  assert(!clock.ready());
  assert(clock.update(true, 60002, valid, true) == ClockAction::Ready);
  assert(clock.update(true, 60003, valid, false) == ClockAction::Ready);
  assert(clock.ready());
  // Clock invalidation disables publishing, even with a previous sync.
  assert(clock.update(true, 60004, 0, false) == ClockAction::Waiting);
  assert(!clock.ready());
  assert(clock.update(false, 70000, valid, false) == ClockAction::Waiting);
  assert(clock.update(true, 70001, valid, false) == ClockAction::StartSync);
  assert(!clock.ready());
  ClockSyncGate wrapped;
  const std::uint32_t start = UINT32_MAX - 10000;
  assert(wrapped.update(true, start, 0, false) == ClockAction::StartSync);
  assert(wrapped.update(true, start + std::uint32_t(30000), 0, false) == ClockAction::TimedOut);
  assert(wrapped.update(true, start + std::uint32_t(60000), 0, false) == ClockAction::StartSync);
  assert(mqttPayloadFits(511, 128));
  assert(!mqttPayloadFits(512, 20));
  assert(!mqttPayloadFits(1, 129));
  assert(!mqttPayloadFits(SIZE_MAX, 128));
  std::cout << "transport policy tests passed\n";
}
