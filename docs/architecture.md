# Arquitetura do protótipo

## Fluxo de dados

1. O módulo GNSS entrega sentenças NMEA ao XIAO ESP32-S3 por UART.
2. O firmware converte latitude e longitude para inteiros com sete casas
   decimais, adiciona identificador, sequência e horário UTC do GNSS.
3. O Wio-SX1262 transmite um pacote binário de 26 bytes em LoRa P2P.
4. A Heltec V2 recebe pelo SX1276, valida versão e CRC e gera uma linha JSON.
5. Nesta primeira etapa o JSON é entregue por USB. Wi-Fi, MQTT e persistência
   serão adicionados após a validação do enlace de rádio.

```text
GNSS -> XIAO ESP32-S3 + Wio-SX1262 -> LoRa P2P 915 MHz
     -> Heltec WiFi LoRa 32 V2 -> USB serial -> computador
```

## Sincronização

O horário absoluto vem do GNSS quando existe fixação válida. A ordenação também
usa um número sequencial por dispositivo. O protótipo mantém ambos os rádios
acordados; portanto, não depende de relógios sincronizados para receber.

Na etapa de baixo consumo, o colar acordará, obterá a posição, transmitirá e
abrirá uma janela de recepção relativa ao fim da transmissão. Isso elimina a
necessidade de despertar colar e receptor no mesmo segundo.

## Limites desta etapa

A Heltec é um receptor central LoRa P2P. Ela não é um gateway LoRaWAN
multicanal. A migração futura para LoRaWAN exigirá um concentrador SX1302 ou
SX1303 e um servidor de rede.
