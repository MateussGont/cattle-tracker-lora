import type { FastifyInstance } from "fastify";
import type { WebSocket } from "ws";

const clients = new Set<WebSocket>();

export type RealtimeEvent =
  | { type: "location_update"; payload: Record<string, unknown> }
  | { type: "alert_created"; payload: Record<string, unknown> };

export function broadcast(event: RealtimeEvent): void {
  const message = JSON.stringify(event);
  for (const client of clients) {
    if (client.readyState === client.OPEN) {
      client.send(message);
    }
  }
}

export async function registerRealtimeGateway(app: FastifyInstance): Promise<void> {
  app.get("/ws", { websocket: true }, (socket, request) => {
    const token = (request.query as { token?: string }).token;
    if (!token) {
      socket.close(4401, "missing token");
      return;
    }

    try {
      app.jwt.verify(token);
    } catch {
      socket.close(4401, "invalid token");
      return;
    }

    clients.add(socket);
    socket.on("close", () => clients.delete(socket));
  });
}
