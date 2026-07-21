# Cattle Tracker LoRa

Protótipo de localização de gado usando um nó compacto no colar e um receptor
central LoRa.

## Hardware inicial

- Colar: Seeed Studio XIAO ESP32-S3 + Wio-SX1262.
- Localização: módulo GNSS NMEA de 3,3 V, como o L76K.
- Receptor central: Heltec WiFi LoRa 32 V2 com SX1276 de 915 MHz.
- Antenas LoRa de 915 MHz conectadas antes de qualquer transmissão.

O SX1262 e o SX1276 são configurados com os mesmos parâmetros físicos: 915 MHz,
bandwidth de 125 kHz, SF7, coding rate 4/5, preâmbulo 8 e rede privada.

## Estado atual

- Firmware do colar lê GNSS e transmite a cada 10 segundos.
- O pacote binário possui identificador, sequência, coordenadas, horário GNSS,
  bateria, flags e CRC-16.
- O receptor valida o pacote e escreve JSON no monitor serial.
- O modo inicial é LoRa ponto a ponto; não é LoRaWAN.
- Deep sleep, ACK, Wi-Fi/MQTT e armazenamento ainda fazem parte do roadmap.

## Ligação do GNSS ao XIAO

| XIAO ESP32-S3 | GNSS |
| --- | --- |
| D7 / GPIO44 (RX) | TX |
| D6 / GPIO43 (TX) | RX, opcional |
| 3V3 | VCC compatível com 3,3 V |
| GND | GND |

Confirme a tensão permitida pelo seu módulo GNSS antes de energizá-lo.

## Compilação com PlatformIO

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

## Primeiro teste

1. Conecte as antenas LoRa corretas.
2. Grave o firmware do receptor e abra o monitor serial.
3. Grave o firmware do colar.
4. Mesmo sem GNSS, o colar envia pacotes com coordenadas zero para validar o
   enlace.
5. Com GNSS conectado e céu aberto, a flag de fixação passa a valer 1 e as
   coordenadas aparecem no JSON.

Exemplo de saída:

```json
{"device_id":1,"sequence":12,"latitude":-19.9231234,"longitude":-43.9401234,"gnss_unix_time":1784635200,"battery_mv":0,"flags":3,"rssi":-87.0,"snr":8.5}
```

Veja [docs/architecture.md](docs/architecture.md) para decisões e limites do
protótipo.

## Referências de hardware

- [XIAO ESP32-S3 + Wio-SX1262](https://wiki.seeedstudio.com/wio_sx1262_with_xiao_esp32s3_kit/)
- [Wio-SX1262](https://wiki.seeedstudio.com/wio_sx1262/)
- [Heltec WiFi LoRa 32 V2](https://resource.heltec.cn/download/Manual%20Old/WiFi%20Lora32Manual.pdf)
- [RadioLib](https://github.com/jgromes/RadioLib)
