# Cattle Tracker LoRa

Sistema de rastreamento e monitoramento de gado: coleiras com ESP32 + GNSS +
LoRa, um gateway que encaminha a telemetria por MQTT, um backend que
processa e persiste os dados, e um aplicativo web para acompanhar o rebanho
no mapa.

```text
Colar (ESP32-S3 + GNSS + LoRa)
  --LoRa P2P 915 MHz-->
Receptor/Gateway (Heltec WiFi LoRa 32 V2)
  --Wi-Fi + MQTT-->
Backend (Node.js + Fastify + TypeScript)
  --SQL / PostGIS-->
PostgreSQL + PostGIS
  --REST + WebSocket-->
Frontend (React + Vite + TypeScript + MapLibre GL)
```

Veja [docs/architecture.md](docs/architecture.md) para as decisões de
arquitetura do sistema completo (banco, API, autenticação, tempo real,
geofencing) e o estado do protótipo de rádio.

## Estrutura do repositório

```text
firmware/     Firmware do colar (ESP32-S3) e do receptor/gateway (Heltec)
backend/      API REST + WebSocket + ingestão de telemetria (Node.js/Fastify)
frontend/     Aplicativo web (React + Vite + MapLibre GL)
infra/        docker-compose para PostgreSQL/PostGIS e Mosquitto (MQTT)
docs/         Documentação técnica
test/         Teste nativo do protocolo binário LoRa (fora do PlatformIO)
```

## Hardware inicial

- Colar: Seeed Studio XIAO ESP32-S3 + Wio-SX1262.
- Localização: módulo GNSS NMEA de 3,3 V, como o L76K.
- Receptor central/gateway: Heltec WiFi LoRa 32 V2 com SX1276 de 915 MHz.
- Antenas LoRa de 915 MHz conectadas antes de qualquer transmissão.

O SX1262 e o SX1276 são configurados com os mesmos parâmetros físicos: 915 MHz,
bandwidth de 125 kHz, SF7, coding rate 4/5, preâmbulo 8 e rede privada.

## Estado atual

- Firmware do colar lê GNSS e transmite a cada 10 segundos.
- O pacote binário possui identificador, sequência, coordenadas, horário GNSS,
  bateria, flags e CRC-16.
- O receptor valida o pacote, publica a telemetria via MQTT (Wi-Fi) e também
  escreve JSON no monitor serial para depuração local.
- O modo de rádio é LoRa ponto a ponto; não é LoRaWAN.
- Deep sleep, ACK e leitura real de bateria (o colar hoje envia `batteryMv=0`
  por falta de circuito de medição) ainda fazem parte do roadmap de hardware.
- Backend, banco de dados e frontend estão implementados (Fases 1–7 da
  arquitetura); ainda não foram validados de ponta a ponta contra hardware
  físico — veja [docs/architecture.md](docs/architecture.md) para o que foi
  testado e o que falta.

## Ligação do GNSS ao XIAO

| XIAO ESP32-S3 | GNSS |
| --- | --- |
| D7 / GPIO44 (RX) | TX |
| D6 / GPIO43 (TX) | RX, opcional |
| 3V3 | VCC compatível com 3,3 V |
| GND | GND |

Confirme a tensão permitida pelo seu módulo GNSS antes de energizá-lo.

## Firmware: compilação com PlatformIO

As dependências são declaradas em `platformio.ini` e serão obtidas pelo
PlatformIO na primeira compilação.

```bash
pio run -e collar_xiao_sx1262
pio run -e receiver_heltec_v2
```

Para gravar cada placa:

```bash
pio run -e collar_xiao_sx1262 -t upload
pio run -e receiver_heltec_v2 -t upload
```

Monitor serial:

```bash
pio device monitor -b 115200
```

### Credenciais Wi-Fi/MQTT do gateway

O receptor precisa de `firmware/receiver/secrets.h` (gitignored) com as
credenciais de Wi-Fi e do broker MQTT:

```bash
cp firmware/receiver/secrets.h.example firmware/receiver/secrets.h
# edite firmware/receiver/secrets.h com o SSID, senha e credenciais MQTT
```

As credenciais MQTT (`MQTT_USERNAME`/`MQTT_PASSWORD`) devem corresponder a um
usuário criado no arquivo `infra/mosquitto/passwd` (veja a seção de infra
abaixo).

### Primeiro teste do enlace LoRa

1. Conecte as antenas LoRa corretas.
2. Grave o firmware do receptor e abra o monitor serial.
3. Grave o firmware do colar.
4. Mesmo sem GNSS, o colar envia pacotes com coordenadas zero para validar o
   enlace.
5. Com GNSS conectado e céu aberto, a flag de fixação passa a valer 1 e as
   coordenadas aparecem no JSON e são publicadas via MQTT.

Exemplo de saída no serial do receptor:

```json
{"device_id":1,"sequence":12,"latitude":-19.9231234,"longitude":-43.9401234,"gnss_unix_time":1784635200,"battery_mv":0,"flags":3,"rssi":-87.0,"snr":8.5}
```

## Infraestrutura local (PostgreSQL/PostGIS + MQTT)

```bash
cd infra
# gera o arquivo de senhas do Mosquitto (uma vez, antes do primeiro `up`)
docker run --rm -v "${PWD}/mosquitto:/mosquitto/config" eclipse-mosquitto:2 \
  mosquitto_passwd -b -c /mosquitto/config/passwd gateway "change-me-local-mqtt"
# No Linux, o arquivo é criado pelo root do container. Torne-o legível pelo
# usuário `mosquitto` antes de subir a infraestrutura:
docker run --rm -v "${PWD}/mosquitto:/work" alpine:3.22 chmod 0644 /work/passwd

docker compose up -d --wait --wait-timeout 60
```

Isso sobe:

- PostgreSQL 16 com PostGIS em `localhost:5432` (usuário/senha/banco
  `cattle_tracker`).
- Mosquitto em `localhost:1883`, exigindo autenticação (sem acesso anônimo).

## Backend

```bash
cd backend
cp .env.example .env   # edite DATABASE_URL, JWT_SECRET, GATEWAY_API_KEY etc.
npm install
npm run db:migrate     # aplica as migrations (cria a extensão PostGIS e as tabelas)
npm run db:seed        # cria um usuário admin e dados de exemplo
npm run dev            # inicia a API em http://localhost:3000
npm test                # roda os testes (vitest)
```

O backend expõe:

- REST em `/api/*` (autenticação JWT, animais, dispositivos, propriedades,
  geofences, alertas, dashboard, mapa).
- `POST /api/telemetry`: fallback HTTP para gateways sem MQTT, autenticado
  pelo header `x-gateway-key` (deve bater com `GATEWAY_API_KEY`).
- `GET /ws`: WebSocket autenticado por token para atualização em tempo real
  do mapa/alertas.
- Consumidor MQTT que assina `MQTT_TELEMETRY_TOPIC` e aplica as mesmas regras
  de validação/negócio do endpoint HTTP.

Veja `backend/.env.example` para todas as variáveis de ambiente.

## Frontend

```bash
cd frontend
cp .env.example .env   # aponte VITE_API_URL/VITE_WS_URL para o backend
npm install
npm run dev             # http://localhost:5173
npm test
npm run build
```

Os scripts `dev` e `build` geram automaticamente a imagem completa do colar
em `frontend/public/firmware/collar-latest.bin` (bootloader, partições e
aplicação) para o assistente Web Serial. Isso exige o PlatformIO disponível no
`PATH`; também é possível gerar o artefato manualmente pela raiz com
`npm run firmware:collar:sync`.

Login inicial (criado pelo `npm run db:seed` do backend):
`admin@cattletracker.local` / `ChangeMe123!` — troque a senha em produção.

O mapa usa MapLibre GL com tiles do OpenStreetMap (gratuitos, sem chave de
API); veja `frontend/src/lib/mapStyle.ts` para trocar por um provedor
dedicado antes de um deploy com tráfego real.

## Referências de hardware

- [XIAO ESP32-S3 + Wio-SX1262](https://wiki.seeedstudio.com/wio_sx1262_with_xiao_esp32s3_kit/)
- [Wio-SX1262](https://wiki.seeedstudio.com/wio_sx1262/)
- [Heltec WiFi LoRa 32 V2](https://resource.heltec.cn/download/Manual%20Old/WiFi%20Lora32Manual.pdf)
- [RadioLib](https://github.com/jgromes/RadioLib)
