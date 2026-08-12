import type { FastifyInstance } from "fastify";
import { createDeviceSchema, listDevicesQuerySchema, updateDeviceSchema } from "../schemas/device.js";
import {
  createDevice,
  findDeviceById,
  listDevices,
  updateDeviceStatus,
} from "../repositories/devicesRepository.js";
import { findCurrentAnimalForDevice } from "../repositories/deviceAssignmentsRepository.js";
import { getStatusThresholds } from "../repositories/settingsRepository.js";
import { authenticate } from "../middlewares/authenticate.js";
import { computeCommunicationStatus } from "../services/statusService.js";

export async function deviceRoutes(app: FastifyInstance): Promise<void> {
  app.addHook("preHandler", authenticate);

  app.get("/api/devices", async (request) => {
    const query = listDevicesQuerySchema.parse(request.query);
    const [thresholds, devices] = await Promise.all([getStatusThresholds(), listDevices(query)]);
    return devices.map((device) => ({
      ...device,
      communicationStatus: computeCommunicationStatus(device.lastSeen, thresholds),
    }));
  });

  app.post("/api/devices", async (request, reply) => {
    if (request.user.role !== "admin") {
      return reply.code(403).send({ error: "forbidden", message: "Apenas administradores podem provisionar dispositivos." });
    }
    const body = createDeviceSchema.parse(request.body);
    const device = await createDevice(body);
    return reply.code(201).send(device);
  });

  app.get("/api/devices/:id", async (request, reply) => {
    const { id } = request.params as { id: string };
    const device = await findDeviceById(id);
    if (!device) {
      return reply.code(404).send({ error: "not_found" });
    }
    const [thresholds, animal] = await Promise.all([getStatusThresholds(), findCurrentAnimalForDevice(id)]);
    return {
      ...device,
      communicationStatus: computeCommunicationStatus(device.lastSeen, thresholds),
      animal,
    };
  });

  app.put("/api/devices/:id", async (request, reply) => {
    if (request.user.role !== "admin") {
      return reply.code(403).send({ error: "forbidden" });
    }
    const { id } = request.params as { id: string };
    const body = updateDeviceSchema.parse(request.body);
    const device = await updateDeviceStatus(id, body);
    if (!device) {
      return reply.code(404).send({ error: "not_found" });
    }
    return device;
  });
}
