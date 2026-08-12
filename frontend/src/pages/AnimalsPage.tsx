import { useState } from "react";
import { Link } from "react-router-dom";
import { LoadingState } from "../components/LoadingState";
import { EmptyState, ErrorState } from "../components/EmptyState";
import { useAnimals } from "../hooks/useAnimals";
import { ApiError } from "../api/client";
import type { AnimalStatus } from "../types";

const STATUS_LABELS: Record<AnimalStatus, string> = {
  active: "Ativo",
  sold: "Vendido",
  deceased: "Morto",
  inactive: "Inativo",
};

export function AnimalsPage() {
  const [search, setSearch] = useState("");
  const [status, setStatus] = useState<AnimalStatus | "">("active");
  const { data, isLoading, error } = useAnimals({ search: search || undefined, status: status || undefined });

  return (
    <div className="p-6">
      <div className="flex items-center justify-between">
        <h1 className="text-2xl font-semibold text-slate-900">Animais</h1>
      </div>

      <div className="mt-4 flex gap-3">
        <input
          type="search"
          placeholder="Buscar por identificação ou nome"
          value={search}
          onChange={(event) => setSearch(event.target.value)}
          className="w-72 rounded-md border border-slate-300 px-3 py-2 text-sm"
        />
        <select
          value={status}
          onChange={(event) => setStatus(event.target.value as AnimalStatus | "")}
          className="rounded-md border border-slate-300 px-3 py-2 text-sm"
        >
          <option value="">Todos os status</option>
          {Object.entries(STATUS_LABELS).map(([value, label]) => (
            <option key={value} value={value}>
              {label}
            </option>
          ))}
        </select>
      </div>

      <div className="mt-6">
        {isLoading && <LoadingState label="Carregando animais..." />}
        {error && <ErrorState message={error instanceof ApiError ? error.message : "Erro desconhecido."} />}
        {data && data.length === 0 && (
          <EmptyState title="Nenhum animal encontrado" description="Ajuste os filtros ou cadastre um novo animal." />
        )}
        {data && data.length > 0 && (
          <div className="overflow-hidden rounded-lg border border-slate-200 bg-white">
            <table className="w-full text-left text-sm">
              <thead className="bg-slate-50 text-xs uppercase text-slate-500">
                <tr>
                  <th className="px-4 py-3">Identificação</th>
                  <th className="px-4 py-3">Nome</th>
                  <th className="px-4 py-3">Status</th>
                  <th className="px-4 py-3" />
                </tr>
              </thead>
              <tbody className="divide-y divide-slate-100">
                {data.map((animal) => (
                  <tr key={animal.id}>
                    <td className="px-4 py-3 font-medium text-slate-900">{animal.tagCode}</td>
                    <td className="px-4 py-3 text-slate-600">{animal.name ?? "—"}</td>
                    <td className="px-4 py-3 text-slate-600">{STATUS_LABELS[animal.status]}</td>
                    <td className="px-4 py-3 text-right">
                      <Link to={`/animals/${animal.id}`} className="text-sm font-medium text-blue-600 hover:underline">
                        Ver detalhes
                      </Link>
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
