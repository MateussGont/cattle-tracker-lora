import { apiRequest } from "./client";
import type { Geofence, Property } from "../types";

export function listProperties(): Promise<Property[]> {
  return apiRequest<Property[]>("/api/properties");
}

export function getProperty(id: string): Promise<Property> {
  return apiRequest<Property>(`/api/properties/${id}`);
}

export function listGeofences(propertyId: string): Promise<Geofence[]> {
  return apiRequest<Geofence[]>(`/api/properties/${propertyId}/geofences`);
}
