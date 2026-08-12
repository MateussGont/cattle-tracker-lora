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
  propertyId?: string | null;
  message: string;
  metadata?: Record<string, unknown>;
}

/** Raises an alert unless one of the same type is already open for the entity, avoiding duplicate spam every telemetry cycle. */
export async function raiseAlertOnce(input: RaiseAlertInput) {
  const existing = await findOpenAlert(input.type, input.deviceId, input.animalId);
  if (existing) {
    return existing;
  }
  return createAlert(input);
}

export async function clearAlert(type: AlertType, deviceId?: string | null, animalId?: string | null) {
  await resolveOpenAlerts(type, deviceId, animalId);
}
