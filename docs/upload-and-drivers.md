# Upload e drivers necessários

## 1. Pré-requisitos de software

Para compilar e gravar os dois firmware com PlatformIO, o ambiente precisa ter:

- Python 3.10 ou 3.11 (x64) instalado no Windows
- PlatformIO Core ou a extensão PlatformIO IDE no VS Code
- Acesso ao porto USB das placas

### Instalação recomendada

No Windows, a forma mais simples é:

1. Instalar Python 3.10/3.11
2. Instalar a extensão PlatformIO IDE no VS Code
3. Reiniciar o VS Code
4. Abrir a pasta do projeto e executar:
   - `pio run -e collar_xiao_sx1262`
   - `pio run -e receiver_heltec_v2`

## 2. Drivers USB necessários

### Placa Heltec WiFi LoRa 32 V2

A Heltec usa um chip USB-to-UART para programação e monitor serial. No Windows, normalmente é necessário instalar o driver:

- Silicon Labs CP210x USB to UART Bridge VCP Driver

Se a porta COM não aparecer em Gerenciador de Dispositivos, esse driver é o primeiro ponto a verificar.

### Placa Seeed XIAO ESP32-S3

O XIAO ESP32-S3 usa a interface USB nativa do ESP32-S3. Em muitos PCs Windows já funciona sem driver adicional, mas se a placa não for detectada, pode ser necessário:

- Driver USB CDC / serial do ESP32-S3
- Atualização do driver USB genérico do Windows para a porta do dispositivo

Se o dispositivo aparecer como "USB Serial Device" ou "Unknown USB Device" no Gerenciador, o problema geralmente está no driver USB ou na seleção da porta correta.

## 3. Drivers de software do projeto

Os drivers de rádio não precisam ser instalados manualmente no sistema operacional. O projeto usa as bibliotecas abaixo, que o PlatformIO baixa automaticamente:

- RadioLib
- TinyGPSPlus

Essas dependências são declaradas em `platformio.ini` e serão resolvidas automaticamente na primeira compilação.

## 4. Fluxo recomendado para upload

### Colar

```bash
pio run -e collar_xiao_sx1262 -t upload
```

### Receptor

```bash
pio run -e receiver_heltec_v2 -t upload
```

### Monitor serial

```bash
pio device monitor -b 115200
```

## 5. Troubleshooting rápido

- Se o upload falhar com erro de porta, verifique a porta COM no Gerenciador de Dispositivos.
- Se o erro for de compilação, confirme se o PlatformIO e as dependências foram instaladas corretamente.
- Se o upload parar no bootloader, tente pressionar o botão BOOT da placa e repetir o upload.
- Se a placa não aparecer, troque o cabo USB ou teste outra porta USB.
