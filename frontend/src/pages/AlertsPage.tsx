import { LoadingState } from "../components/LoadingState";
import { EmptyState, ErrorState } from "../components/EmptyState";
import { useAlerts, useUpdateAlertStatus } from "../hooks/useAlerts";
import { ApiError } from "../api/client";
import type { AlertSeverity } from "../types";

const SEVERITY_CLASSES: Record<AlertSeverity, string> = {
  info: "bg-blue-100 text-blue-700",
  warning: "bg-amber-100 text-amber-700",
  critical: "bg-red-100 text-red-700",
};

export function AlertsPage() {
  const { data, isLoading, error } = useAlerts({ status: "open" });
  const updateStatus = useUpdateAlertStatus();

  return (
    <div className="p-6">
      <h1 className="text-2xl font-semibold text-slate-900">Alertas</h1>

      <div className="mt-6 space-y-3">
        {isLoading && <LoadingState label="Carregando alertas..." />}
        {error && <ErrorState message={error instanceof ApiError ? error.message : "Erro desconhecido."} />}
        {data && data.length === 0 && <EmptyState title="Nenhum alerta em aberto" description="Tudo certo por aqui." />}
        {data?.map((alert) => (
          <div key={alert.id} className="flex items-center justify-between rounded-lg border border-slate-200 bg-white p-4">
            <div>
              <span className={`inline-block rounded-full px-2 py-0.5 text-xs font-medium ${SEVERITY_CLASSES[alert.severity]}`}>
                {alert.severity}
              </span>
              <p className="mt-1 text-sm text-slate-800">{alert.message}</p>
              <p className="text-xs text-slate-500">{new Date(alert.triggeredAt).toLocaleString("pt-BR")}</p>
            </div>
            <div className="flex gap-2">
              <button
                type="button"
                onClick={() => updateStatus.mutate({ id: alert.id, status: "acknowledged" })}
                className="rounded-md border border-slate-300 px-3 py-1.5 text-sm text-slate-700 hover:bg-slate-100"
              >
                Reconhecer
              </button>
              <button
                type="button"
                onClick={() => updateStatus.mutate({ id: alert.id, status: "resolved" })}
                className="rounded-md bg-slate-900 px-3 py-1.5 text-sm text-white hover:bg-slate-800"
              >
                Resolver
              </button>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
