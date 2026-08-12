import { LoadingState } from "../components/LoadingState";
import { EmptyState, ErrorState } from "../components/EmptyState";
import { StatusBadge } from "../components/StatusBadge";
import { useDevices } from "../hooks/useDevices";
import { ApiError } from "../api/client";

export function DevicesPage() {
  const { data, isLoading, error } = useDevices();

  return (
    <div className="p-6">
      <h1 className="text-2xl font-semibold text-slate-900">Dispositivos</h1>

      <div className="mt-6">
        {isLoading && <LoadingState label="Carregando dispositivos..." />}
        {error && <ErrorState message={error instanceof ApiError ? error.message : "Erro desconhecido."} />}
        {data && data.length === 0 && <EmptyState title="Nenhum dispositivo cadastrado" />}
        {data && data.length > 0 && (
          <div className="overflow-hidden rounded-lg border border-slate-200 bg-white">
            <table className="w-full text-left text-sm">
              <thead className="bg-slate-50 text-xs uppercase text-slate-500">
                <tr>
                  <th className="px-4 py-3">ID</th>
                  <th className="px-4 py-3">Comunicação</th>
                  <th className="px-4 py-3">Bateria</th>
                  <th className="px-4 py-3">Última comunicação</th>
                </tr>
              </thead>
              <tbody className="divide-y divide-slate-100">
                {data.map((device) => (
                  <tr key={device.id}>
                    <td className="px-4 py-3 font-medium text-slate-900">{device.deviceIdentifier}</td>
                    <td className="px-4 py-3">
                      {device.communicationStatus && <StatusBadge status={device.communicationStatus} />}
                    </td>
                    <td className="px-4 py-3 text-slate-600">{device.batteryLevel ?? "—"}%</td>
                    <td className="px-4 py-3 text-slate-600">
                      {device.lastSeen ? new Date(device.lastSeen).toLocaleString("pt-BR") : "Nunca"}
                    </td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        )}
      </div>
    </div>
  );
}
