import { eq } from "drizzle-orm";
import { db } from "../db/client.js";
import { gateways } from "../db/schema.js";

export async function listGateways(propertyIds?: string[]) {
  if (!propertyIds) {
    return db.select().from(gateways).orderBy(gateways.name);
  }
  const rows = await db.select().from(gateways).orderBy(gateways.name);
  return rows.filter((row) => row.propertyId === null || propertyIds.includes(row.propertyId));
}

export async function findGatewayById(id: string) {
  const [gateway] = await db.select().from(gateways).where(eq(gateways.id, id)).limit(1);
  return gateway ?? null;
}

export interface CreateGatewayInput {
  name: string;
  gatewayIdentifier: string;
  propertyId?: string;
}

export async function createGateway(input: CreateGatewayInput) {
  const [gateway] = await db.insert(gateways).values(input).returning();
  return gateway;
}

export async function touchGatewayLastSeen(gatewayIdentifier: string): Promise<void> {
  await db
    .update(gateways)
    .set({ lastSeen: new Date(), updatedAt: new Date() })
    .where(eq(gateways.gatewayIdentifier, gatewayIdentifier));
}
