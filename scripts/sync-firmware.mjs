// Merges the bootloader, partition table, OTA bootstrap and application into
// the single image consumed by the browser's Web Serial provisioning wizard.
import { execFileSync } from "node:child_process";
import { existsSync, mkdirSync, statSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const repoRoot = dirname(dirname(fileURLToPath(import.meta.url)));
const buildDir = join(repoRoot, ".pio", "build", "collar_xiao_sx1262");
const bootloader = join(buildDir, "bootloader.bin");
const partitions = join(buildDir, "partitions.bin");
const application = join(buildDir, "firmware.bin");
const destDir = join(repoRoot, "frontend", "public", "firmware");
const dest = join(destDir, "collar-latest.bin");

for (const source of [bootloader, partitions, application]) {
  if (!existsSync(source)) {
    console.error(`Firmware image not found at ${source}. Run "pio run -e collar_xiao_sx1262" first.`);
    process.exit(1);
  }
}

const systemInfo = JSON.parse(
  execFileSync("pio", ["system", "info", "--json-output"], { encoding: "utf8" }),
);
const coreDir = systemInfo.core_dir?.value;
const python = systemInfo.python_exe?.value;
if (!coreDir || !python) {
  throw new Error("Could not determine the PlatformIO core/python paths.");
}

const esptool = join(coreDir, "packages", "tool-esptoolpy", "esptool.py");
const bootApp = join(
  coreDir,
  "packages",
  "framework-arduinoespressif32",
  "tools",
  "partitions",
  "boot_app0.bin",
);
for (const source of [esptool, bootApp]) {
  if (!existsSync(source)) {
    throw new Error(`Required PlatformIO package file not found: ${source}`);
  }
}

mkdirSync(destDir, { recursive: true });
execFileSync(
  python,
  [
    esptool,
    "--chip", "esp32s3",
    "merge_bin",
    "-o", dest,
    "--flash_mode", "dio",
    "--flash_freq", "80m",
    "--flash_size", "8MB",
    "0x0", bootloader,
    "0x8000", partitions,
    "0xe000", bootApp,
    "0x10000", application,
  ],
  { stdio: "inherit" },
);

const size = statSync(dest).size;
if (size <= statSync(application).size) {
  throw new Error(`Merged firmware is unexpectedly small (${size} bytes).`);
}
console.log(`Merged flash image (${size} bytes) -> ${dest}`);
