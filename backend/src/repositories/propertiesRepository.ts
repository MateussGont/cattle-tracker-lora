import { eq, sql } from "drizzle-orm";
import { db } from "../db/client.js";
import { properties } from "../db/schema.js";
import { parseGeoJson, pointToWkt, polygonToWkt, type LatLng } from "../utils/geo.js";
import type { GeoJsonPoint, GeoJsonPolygon } from "../schemas/geoJson.js";

const propertyGeoJsonSelect = {
  id: properties.id,
  name: properties.name,
  areaHectares: properties.areaHectares,
  location: sql<string | null>`ST_AsGeoJSON(${properties.location})`.as("location"),
  boundary: sql<string | null>`ST_AsGeoJSON(${properties.boundary})`.as("boundary"),
  createdAt: properties.createdAt,
  updatedAt: properties.updatedAt,
};

function withParsedGeometry<T extends { location: string | null; boundary: string | null }>(row: T) {
  return {
    ...row,
    location: parseGeoJson<GeoJsonPoint>(row.location),
    boundary: parseGeoJson<GeoJsonPolygon>(row.boundary),
  };
}

export async function listProperties(propertyIds?: string[]) {
  const query = db.select(propertyGeoJsonSelect).from(properties);
  const rows = propertyIds && propertyIds.length > 0
    ? await query.where(sql`${properties.id} = ANY(${propertyIds})`)
    : await query;
  return rows.map(withParsedGeometry);
}

export async function findPropertyById(id: string) {
  const [property] = await db
    .select(propertyGeoJsonSelect)
    .from(properties)
    .where(eq(properties.id, id))
    .limit(1);
  return property ? withParsedGeometry(property) : null;
}

export interface CreatePropertyInput {
  name: string;
  location?: LatLng;
  boundary?: LatLng[];
}

export async function createProperty(input: CreatePropertyInput) {
  const [property] = await db
    .insert(properties)
    .values({
      name: input.name,
      location: input.location
        ? sql`ST_SetSRID(ST_GeomFromText(${pointToWkt(input.location)}), 4326)::geography`
        : undefined,
      boundary: input.boundary
        ? sql`ST_SetSRID(ST_GeomFromText(${polygonToWkt(input.boundary)}), 4326)`
        : undefined,
    })
    .returning({ id: properties.id });
  return property;
}
