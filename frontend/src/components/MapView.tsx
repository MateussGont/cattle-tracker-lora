import maplibregl from "maplibre-gl";
import "maplibre-gl/dist/maplibre-gl.css";
import { useEffect, useRef } from "react";
import { osmRasterStyle } from "../lib/mapStyle";
import type { CommunicationStatus } from "../types";

export interface MapMarkerData {
  id: string;
  latitude: number;
  longitude: number;
  status: CommunicationStatus;
  popupHtml: string;
}

interface MapViewProps {
  markers: MapMarkerData[];
  trajectory?: [number, number][];
  fallbackCenter?: [number, number];
}

const STATUS_COLORS: Record<CommunicationStatus, string> = {
  online: "#16a34a",
  attention: "#d97706",
  offline: "#dc2626",
  never_seen: "#6b7280",
};

const DEFAULT_CENTER: [number, number] = [-44.012345, -19.923456];
const TRAJECTORY_SOURCE_ID = "trajectory";

export function MapView({ markers, trajectory, fallbackCenter = DEFAULT_CENTER }: MapViewProps) {
  const containerRef = useRef<HTMLDivElement | null>(null);
  const mapRef = useRef<maplibregl.Map | null>(null);
  const markerInstancesRef = useRef<maplibregl.Marker[]>([]);

  useEffect(() => {
    if (!containerRef.current) return;

    const map = new maplibregl.Map({
      container: containerRef.current,
      style: osmRasterStyle,
      center: fallbackCenter,
      zoom: 13,
    });
    map.addControl(new maplibregl.NavigationControl(), "top-right");
    mapRef.current = map;

    return () => {
      map.remove();
      mapRef.current = null;
    };
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  useEffect(() => {
    const map = mapRef.current;
    if (!map) return;

    for (const marker of markerInstancesRef.current) {
      marker.remove();
    }
    markerInstancesRef.current = [];

    const bounds = new maplibregl.LngLatBounds();
    let hasPoint = false;

    for (const item of markers) {
      const el = document.createElement("div");
      el.style.width = "16px";
      el.style.height = "16px";
      el.style.borderRadius = "50%";
      el.style.border = "2px solid white";
      el.style.boxShadow = "0 0 0 1px rgba(0,0,0,0.2)";
      el.style.backgroundColor = STATUS_COLORS[item.status];
      el.style.cursor = "pointer";

      const popup = new maplibregl.Popup({ offset: 12 }).setHTML(item.popupHtml);
      const marker = new maplibregl.Marker({ element: el })
        .setLngLat([item.longitude, item.latitude])
        .setPopup(popup)
        .addTo(map);

      markerInstancesRef.current.push(marker);
      bounds.extend([item.longitude, item.latitude]);
      hasPoint = true;
    }

    if (trajectory && trajectory.length > 0) {
      for (const point of trajectory) {
        bounds.extend(point);
      }
      hasPoint = true;
    }

    if (hasPoint) {
      map.fitBounds(bounds, { padding: 60, maxZoom: 16, duration: 0 });
    }
  }, [markers, trajectory]);

  useEffect(() => {
    const map = mapRef.current;
    if (!map) return;

    function applyTrajectory(target: maplibregl.Map) {
      if (target.getLayer(TRAJECTORY_SOURCE_ID)) {
        target.removeLayer(TRAJECTORY_SOURCE_ID);
      }
      if (target.getSource(TRAJECTORY_SOURCE_ID)) {
        target.removeSource(TRAJECTORY_SOURCE_ID);
      }

      if (!trajectory || trajectory.length < 2) {
        return;
      }

      target.addSource(TRAJECTORY_SOURCE_ID, {
        type: "geojson",
        data: { type: "Feature", properties: {}, geometry: { type: "LineString", coordinates: trajectory } },
      });
      target.addLayer({
        id: TRAJECTORY_SOURCE_ID,
        type: "line",
        source: TRAJECTORY_SOURCE_ID,
        paint: { "line-color": "#2563eb", "line-width": 3 },
      });
    }

    if (map.isStyleLoaded()) {
      applyTrajectory(map);
    } else {
      map.once("load", () => applyTrajectory(map));
    }
  }, [trajectory]);

  return <div ref={containerRef} className="h-full w-full" />;
}
