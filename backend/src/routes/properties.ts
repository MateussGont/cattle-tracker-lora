import type { FastifyInstance } from "fastify";
import { createGeofenceSchema, createPropertySchema } from "../schemas/property.js";
import {
  createProperty,
  findPropertyById,
  listProperties,
} from "../repositories/propertiesRepository.js";
import { createGeofence, listGeofencesByProperty } from "../repositories/geofencesRepository.js";
import { accessiblePropertyIds, assertPropertyAccess, authenticate } from "../middlewares/authenticate.js";

export async function propertyRoutes(app: FastifyInstance): Promise<void> {
  app.addHook("preHandler", authenticate);

  app.get("/api/properties", async (request) => {
    const propertyIds = await accessiblePropertyIds(request);
    return listProperties(propertyIds);
  });

  app.post("/api/properties", async (request, reply) => {
    if (request.user.role !== "admin") {
      return reply.code(403).send({ error: "forbidden" });
    }
    const body = createPropertySchema.parse(request.body);
    const property = await createProperty(body);
    return reply.code(201).send(property);
  });

  app.get("/api/properties/:id", async (request, reply) => {
    const { id } = request.params as { id: string };
    if (!(await assertPropertyAccess(request, id))) {
      return reply.code(403).send({ error: "forbidden" });
    }
    const property = await findPropertyById(id);
    return property ?? reply.code(404).send({ error: "not_found" });
  });

  app.get("/api/properties/:id/geofences", async (request, reply) => {
    const { id } = request.params as { id: string };
    if (!(await assertPropertyAccess(request, id))) {
      return reply.code(403).send({ error: "forbidden" });
    }
    return listGeofencesByProperty(id);
  });

  app.post("/api/properties/:id/geofences", async (request, reply) => {
    const { id } = request.params as { id: string };
    if (!(await assertPropertyAccess(request, id))) {
      return reply.code(403).send({ error: "forbidden" });
    }
    const body = createGeofenceSchema.parse(request.body);
    const geofence = await createGeofence({ ...body, propertyId: id });
    return reply.code(201).send(geofence);
  });
}
