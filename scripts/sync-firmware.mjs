// Copies the merged, flashable collar firmware image built by PlatformIO
// into frontend/public so the browser (Web Serial provisioning wizard) can
// fetch it as a static asset. Run after any change to firmware/collar or
// firmware/common: `pio run -e collar_xiao_sx1262 && node scripts/sync-firmware.mjs`.
import { copyFileSync, existsSync, mkdirSync } from "node:fs";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

const repoRoot = dirname(dirname(fileURLToPath(import.meta.url)));
const source = join(repoRoot, ".pio", "build", "collar_xiao_sx1262", "firmware.bin");
const destDir = join(repoRoot, "frontend", "public", "firmware");
const dest = join(destDir, "collar-latest.bin");

if (!existsSync(source)) {
  console.error(`Firmware image not found at ${source}. Run "pio run -e collar_xiao_sx1262" first.`);
  process.exit(1);
}

mkdirSync(destDir, { recursive: true });
copyFileSync(source, dest);
console.log(`Copied ${source} -> ${dest}`);
