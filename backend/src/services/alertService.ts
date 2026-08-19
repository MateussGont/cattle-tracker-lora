import {
  createAlert,
  findOpenAlert,
  resolveOpenAlerts,
  type AlertSeverity,
  type AlertType,
} from "../repositories/alertsRepository.js";

export interface RaiseAlertInput {
  type: AlertType;
  severity: AlertSeverity;
  animalId?: string | null;
  deviceId?: string | null;
  gatewayId?: string | null;
  propertyId?: string | null;
  ruleId?: string | null;
  message: string;
  metadata?: Record<string, unknown>;
}

/** Raises an alert unless one of the same type is already open for the entity, avoiding duplicate spam every telemetry cycle. */
export async function raiseAlertOnce(input: RaiseAlertInput) {
  const existing = await findOpenAlert({
    type: input.type,
    deviceId: input.deviceId,
    animalId: input.animalId,
    gatewayId: input.gatewayId,
    ruleId: input.ruleId,
  });
  if (existing) {
    return existing;
  }
  return createAlert(input);
}

export interface ClearAlertInput {
  type: AlertType;
  deviceId?: string | null;
  animalId?: string | null;
  gatewayId?: string | null;
  ruleId?: string | null;
}

export async function clearAlert(key: ClearAlertInput) {
  await resolveOpenAlerts(key);
}
