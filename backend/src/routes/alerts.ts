import type { FastifyInstance } from "fastify";
import { listAlertsQuerySchema, updateAlertSchema } from "../schemas/alert.js";
import { listAlerts, updateAlertStatus } from "../repositories/alertsRepository.js";
import { accessiblePropertyIds, authenticate } from "../middlewares/authenticate.js";

export async function alertRoutes(app: FastifyInstance): Promise<void> {
  app.addHook("preHandler", authenticate);

  app.get("/api/alerts", async (request) => {
    const query = listAlertsQuerySchema.parse(request.query);
    const propertyIds = await accessiblePropertyIds(request);
    return listAlerts({ ...query, propertyIds });
  });

  app.put("/api/alerts/:id", async (request, reply) => {
    const { id } = request.params as { id: string };
    const body = updateAlertSchema.parse(request.body);
    const alert = await updateAlertStatus(id, body.status);
    return alert ?? reply.code(404).send({ error: "not_found" });
  });
}
