import { useQuery } from "@tanstack/react-query";
import { getProperty, listGeofences, listProperties } from "../api/properties";

export function useProperties() {
  return useQuery({ queryKey: ["properties"], queryFn: listProperties });
}

export function useProperty(id: string | undefined) {
  return useQuery({
    queryKey: ["property", id],
    queryFn: () => getProperty(id as string),
    enabled: Boolean(id),
  });
}

export function useGeofences(propertyId: string | undefined) {
  return useQuery({
    queryKey: ["geofences", propertyId],
    queryFn: () => listGeofences(propertyId as string),
    enabled: Boolean(propertyId),
  });
}
