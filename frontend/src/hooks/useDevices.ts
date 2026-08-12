import { useMutation, useQuery, useQueryClient } from "@tanstack/react-query";
import { createDevice, getDevice, listDevices, type CreateDeviceInput, type ListDevicesParams } from "../api/devices";

export function useDevices(params: ListDevicesParams = {}) {
  return useQuery({ queryKey: ["devices", params], queryFn: () => listDevices(params) });
}

export function useDevice(id: string | undefined) {
  return useQuery({
    queryKey: ["device", id],
    queryFn: () => getDevice(id as string),
    enabled: Boolean(id),
  });
}

export function useCreateDevice() {
  const queryClient = useQueryClient();
  return useMutation({
    mutationFn: (input: CreateDeviceInput) => createDevice(input),
    onSuccess: () => void queryClient.invalidateQueries({ queryKey: ["devices"] }),
  });
}
