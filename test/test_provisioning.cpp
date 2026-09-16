#include <cassert>
#include <iostream>

#include "../firmware/common/provisioning.h"

int main() {
  using cattle_tracker::ProvisionDecision;

  assert(cattle_tracker::decideProvisioning(nullptr, 101, 1) ==
         ProvisionDecision::kApply);
  assert(cattle_tracker::decideProvisioning(nullptr, 0, 1) ==
         ProvisionDecision::kInvalidRadioDeviceId);
  assert(cattle_tracker::decideProvisioning(nullptr, 101, 0) ==
         ProvisionDecision::kInvalidRevision);

  const auto current = cattle_tracker::makeDeviceConfig(101, 1);
  assert(cattle_tracker::isValidDeviceConfig(current));
  assert(cattle_tracker::decideProvisioning(&current, 101, 1) ==
         ProvisionDecision::kIdempotent);
  assert(cattle_tracker::decideProvisioning(&current, 102, 2) ==
         ProvisionDecision::kAlreadyProvisioned);

  auto corrupted = current;
  corrupted.radioDeviceId = 102;
  assert(!cattle_tracker::isValidDeviceConfig(corrupted));

  std::cout << "provisioning tests passed\n";
  return 0;
}
