import { eq } from "drizzle-orm";
import { db } from "../db/client.js";
import { users, userProperties } from "../db/schema.js";

export async function findUserByEmail(email: string) {
  const [user] = await db.select().from(users).where(eq(users.email, email)).limit(1);
  return user ?? null;
}

export async function findUserById(id: string) {
  const [user] = await db.select().from(users).where(eq(users.id, id)).limit(1);
  return user ?? null;
}

export async function getUserPropertyIds(userId: string): Promise<string[]> {
  const rows = await db
    .select({ propertyId: userProperties.propertyId })
    .from(userProperties)
    .where(eq(userProperties.userId, userId));
  return rows.map((row) => row.propertyId);
}

export async function hasPropertyAccess(userId: string, propertyId: string): Promise<boolean> {
  const propertyIds = await getUserPropertyIds(userId);
  return propertyIds.includes(propertyId);
}
