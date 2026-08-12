import { and, desc, eq, isNull, or, sql } from "drizzle-orm";
import { db } from "../db/client.js";
import { alerts } from "../db/schema.js";

export type AlertType =
  | "geofence_exit"
  | "device_offline"
  | "low_battery"
  | "gps_stale"
  | "no_communication"
  | "other";
export type AlertSeverity = "info" | "warning" | "critical";
export type AlertStatus = "open" | "acknowledged" | "resolved";

export interface CreateAlertInput {
  type: AlertType;
  severity: AlertSeverity;
  animalId?: string | null;
  deviceId?: string | null;
  propertyId?: string | null;
  message: string;
  metadata?: Record<string, unknown>;
}

/**
 * Finds an already-open alert of the same type for the same entity, so
 * callers can avoid spamming a new row every telemetry cycle while the
 * underlying condition (e.g. low battery) persists.
 */
export async function findOpenAlert(type: AlertType, deviceId?: string | null, animalId?: string | null) {
  const conditions = [eq(alerts.type, type), eq(alerts.status, "open")];
  if (deviceId) conditions.push(eq(alerts.deviceId, deviceId));
  if (animalId) conditions.push(eq(alerts.animalId, animalId));

  const [alert] = await db.select().from(alerts).where(and(...conditions)).limit(1);
  return alert ?? null;
}

export async function createAlert(input: CreateAlertInput) {
  const [alert] = await db
    .insert(alerts)
    .values({
      type: input.type,
      severity: input.severity,
      animalId: input.animalId ?? null,
      deviceId: input.deviceId ?? null,
      propertyId: input.propertyId ?? null,
      message: input.message,
      metadata: input.metadata ?? null,
    })
    .returning();
  return alert;
}

export async function resolveOpenAlerts(type: AlertType, deviceId?: string | null, animalId?: string | null) {
  const conditions = [eq(alerts.type, type), eq(alerts.status, "open")];
  if (deviceId) conditions.push(eq(alerts.deviceId, deviceId));
  if (animalId) conditions.push(eq(alerts.animalId, animalId));

  await db
    .update(alerts)
    .set({ status: "resolved", resolvedAt: new Date(), updatedAt: new Date() })
    .where(and(...conditions));
}

export interface ListAlertsFilter {
  propertyIds?: string[];
  propertyId?: string;
  status?: AlertStatus;
  limit: number;
  offset: number;
}

export async function listAlerts(filter: ListAlertsFilter) {
  const conditions = [];
  if (filter.propertyId) {
    conditions.push(eq(alerts.propertyId, filter.propertyId));
  } else if (filter.propertyIds) {
    conditions.push(
      or(sql`${alerts.propertyId} = ANY(${filter.propertyIds})`, isNull(alerts.propertyId)),
    );
  }
  if (filter.status) {
    conditions.push(eq(alerts.status, filter.status));
  }

  return db
    .select()
    .from(alerts)
    .where(conditions.length ? and(...conditions) : undefined)
    .orderBy(desc(alerts.triggeredAt))
    .limit(filter.limit)
    .offset(filter.offset);
}

export async function updateAlertStatus(id: string, status: AlertStatus) {
  const [alert] = await db
    .update(alerts)
    .set({
      status,
      resolvedAt: status === "resolved" ? new Date() : null,
      updatedAt: new Date(),
    })
    .where(eq(alerts.id, id))
    .returning();
  return alert ?? null;
}
