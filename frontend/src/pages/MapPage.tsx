import { useMemo } from "react";
import { LoadingState } from "../components/LoadingState";
import { ErrorState } from "../components/EmptyState";
import { MapView, type MapMarkerData } from "../components/MapView";
import { useMapAnimals } from "../hooks/useDashboard";
import { ApiError } from "../api/client";

function formatDateTime(value: string | null): string {
  if (!value) return "Nunca";
  return new Date(value).toLocaleString("pt-BR");
}

function buildPopupHtml(marker: { animalId: string; tagCode: string; name: string | null; batteryLevel: number | null; latitude: number | null; longitude: number | null; lastSeen: string | null }): string {
  const title = marker.name ? `${marker.name} (${marker.tagCode})` : marker.tagCode;
  return `
    <div style="font-family: system-ui, sans-serif; min-width: 200px;">
      <p style="font-weight: 600; margin: 0 0 4px;">${title}</p>
      <p style="margin: 0; font-size: 12px; color: #475569;">Última localização: ${formatDateTime(marker.lastSeen)}</p>
      <p style="margin: 0; font-size: 12px; color: #475569;">Bateria: ${marker.batteryLevel ?? "—"}%</p>
      <p style="margin: 0; font-size: 12px; color: #475569;">Lat: ${marker.latitude?.toFixed(6) ?? "—"} · Lon: ${marker.longitude?.toFixed(6) ?? "—"}</p>
      <div style="margin-top: 8px; display: flex; gap: 8px;">
        <a href="/animals/${marker.animalId}" style="font-size: 12px; color: #2563eb; font-weight: 600;">VER DETALHES</a>
        <a href="/history?animalId=${marker.animalId}" style="font-size: 12px; color: #2563eb; font-weight: 600;">VER HISTÓRICO</a>
      </div>
    </div>
  `;
}

export function MapPage() {
  const { data, isLoading, error } = useMapAnimals();

  const markers = useMemo<MapMarkerData[]>(() => {
    if (!data) return [];
    return data
      .filter((marker) => marker.latitude !== null && marker.longitude !== null)
      .map((marker) => ({
        id: marker.animalId,
        latitude: marker.latitude as number,
        longitude: marker.longitude as number,
        status: marker.communicationStatus,
        popupHtml: buildPopupHtml(marker),
      }));
  }, [data]);

  return (
    <div className="flex h-full flex-col">
      <div className="border-b border-slate-200 bg-white px-6 py-4">
        <h1 className="text-xl font-semibold text-slate-900">Mapa</h1>
        <p className="text-sm text-slate-500">{markers.length} animal(is) com posição conhecida.</p>
      </div>
      <div className="flex-1">
        {isLoading && <LoadingState label="Carregando posições..." />}
        {error && <ErrorState message={error instanceof ApiError ? error.message : "Erro desconhecido."} />}
        {data && <MapView markers={markers} />}
      </div>
    </div>
  );
}
