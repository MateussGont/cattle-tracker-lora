import { LoadingState } from "../components/LoadingState";
import { EmptyState, ErrorState } from "../components/EmptyState";
import { useProperties } from "../hooks/useProperties";
import { ApiError } from "../api/client";

export function PropertiesPage() {
  const { data, isLoading, error } = useProperties();

  return (
    <div className="p-6">
      <h1 className="text-2xl font-semibold text-slate-900">Propriedades</h1>

      <div className="mt-6 grid grid-cols-1 gap-4 sm:grid-cols-2 lg:grid-cols-3">
        {isLoading && <LoadingState label="Carregando propriedades..." />}
        {error && <ErrorState message={error instanceof ApiError ? error.message : "Erro desconhecido."} />}
        {data && data.length === 0 && <EmptyState title="Nenhuma propriedade cadastrada" />}
        {data?.map((property) => (
          <div key={property.id} className="rounded-lg border border-slate-200 bg-white p-4">
            <p className="font-medium text-slate-900">{property.name}</p>
            <p className="mt-1 text-sm text-slate-500">
              {property.areaHectares ? `${property.areaHectares} hectares` : "Área não informada"}
            </p>
          </div>
        ))}
      </div>
    </div>
  );
}
