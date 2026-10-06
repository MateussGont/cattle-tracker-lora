// No credentials: verifies the public broker using exactly the firmware CA bundle.
import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { X509Certificate } from 'node:crypto';
import tls from 'node:tls';
const header = readFileSync(new URL('../firmware/receiver/mqtt_root_ca.h', import.meta.url), 'utf8');
const certificates = header.match(/-----BEGIN CERTIFICATE-----[\s\S]*?-----END CERTIFICATE-----/g);
assert.equal(certificates?.length, 2);
const expected = [
  '96:BC:EC:06:26:49:76:F3:74:60:77:9A:CF:28:C5:A7:CF:E8:A3:C0:AA:E1:1A:8F:FC:EE:05:C0:BD:DF:08:C6',
  '69:72:9B:8E:15:A8:6E:FC:17:7A:57:AF:B7:17:1D:FC:64:AD:D2:8C:2F:CA:8C:F1:50:7E:34:45:3C:CB:14:70',
];
for (const [index, pem] of certificates.entries()) {
  const cert = new X509Certificate(pem);
  assert.ok(cert.ca);
  assert.equal(cert.fingerprint256, expected[index]);
}
const publisher = readFileSync(new URL('../firmware/receiver/mqtt_publisher.cpp', import.meta.url), 'utf8');
assert.ok(publisher.includes('setCACert(kMqttRootCa)'));
assert.ok(!publisher.includes('setInsecure('));
const host = process.argv[2] ?? 'cattletracker.tech';
function handshake(options = {}) {
  return new Promise((resolve, reject) => {
    const socket = tls.connect({ host, port: 8883, servername: host, ca: certificates, rejectUnauthorized: true, minVersion: 'TLSv1.2', maxVersion: 'TLSv1.2', ...options });
    socket.setTimeout(8000, () => socket.destroy(Error('TLS timeout')));
    socket.once('error', reject);
    socket.once('secureConnect', () => {
      const result = { protocol: socket.getProtocol(), validTo: socket.getPeerCertificate().valid_to };
      socket.end(); resolve(result);
    });
  });
}
console.log(await handshake());
await assert.rejects(handshake({ servername: 'invalid.example.test' }), error => error.code === 'ERR_TLS_CERT_ALTNAME_INVALID');
await assert.rejects(handshake({ ca: [] }));
console.log('Firmware CA fingerprints and broker TLS verified; wrong hostname and untrusted CA refused. Not a hardware test.');
