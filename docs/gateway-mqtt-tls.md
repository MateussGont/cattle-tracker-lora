# Configurar o gateway para a VPS com MQTT TLS

Recorte da [issue #34](https://github.com/MateussGont/cattle-tracker-lora/issues/34), preparado em **2026-10-05**. Servidor: [web PR #29](https://github.com/MateussGont/cattle-tracker-web/pull/29). Não altera o pacote LoRa, tópico ou JSON de telemetria.

## Configuração do equipamento

1. Atualizar o repositório para a `main` que contém esta entrega.
2. Copiar `firmware/receiver/secrets.h.example` para `secrets.h`, ignorado pelo Git. Se já existir, editar sem sobrescrever as credenciais inadvertidamente.
3. Preencher Wi-Fi, `MQTT_HOST=cattletracker.tech`, `MQTT_PORT=8883`, usuário MQTT, senha e `GATEWAY_ID` único. O host não deve conter `https://`, caminho ou endereço IP.
4. Solicitar somente a credencial MQTT ao responsável pela VPS. O arquivo privado de entrega é `/opt/cattle-tracker/shared/gateway-mqtt.json`; não compartilhar SSH/root/admin do site nem chave privada TLS.
5. Conferir placa Heltec WiFi LoRa 32 V2 e antena apropriada antes de transmitir. Compilar e gravar:

```text
pio run -e receiver_heltec_v2
pio run -e receiver_heltec_v2 -t upload
pio device monitor -b 115200
```

Não publicar `secrets.h` nem binários do gateway com credenciais. A compilação de validação desta entrega usa somente placeholders: não é um firmware já configurado para sua rede.

## Comportamento implementado

- `WiFiClientSecure` valida a cadeia, hostname e datas do certificado. Raízes públicas ISRG X1/X2 incluídas em `mqtt_root_ca.h`; não há `setInsecure`, uso do certificado temporário do servidor como raiz ou fallback para MQTT sem TLS.
- Porta 8883 obrigatória neste recorte; endereço IPv4 literal/configuração básica inválida deixam o transporte desabilitado. A verificação final do hostname é feita pela biblioteca TLS na conexão por nome.
- Após Wi-Fi conectado, `configTime` inicia SNTP sem espera ativa. MQTT só conecta depois de uma sincronização confirmada e horário a partir de 2026-01-01. Um relógio plausível sem confirmação não basta.
- Espera de SNTP: 30 s por tentativa, nova solicitação após 60 s. Falha mantém MQTT desabilitado e não interrompe permanentemente o loop. O cliente SNTP pode concluir entre essas janelas. Cada reconexão Wi-Fi exige sincronização novamente.
- TCP/TLS/MQTT continuam síncronos: timeout TCP/leitura de 5 s, handshake TLS de 8 s e espera MQTT de 5 s; DNS segue os limites da biblioteca. Backoff MQTT de 5 s contado após o término da tentativa. **Isso não torna a recepção LoRa assíncrona**: perdas durante tentativas ainda são possíveis ([#5](https://github.com/MateussGont/cattle-tracker-lora/issues/5)).
- Buffer MQTT de 1024 bytes, payload JSON de até 511 bytes e tópico de até 128 bytes. Verifica tamanho antes de serializar e nunca publica JSON truncado. `publish` continua QoS0; sucesso local não é confirmação de persistência no backend.
- Logs não imprimem senha, certificado privado ou SSID. Estado MQTT e timeout de SNTP ajudam no diagnóstico.

SNTP usa `time.cloudflare.com`, `pool.ntp.org` e `time.google.com`: rede precisa permitir DNS e NTP/UDP123, além de TCP8883. SNTP não é uma fonte de tempo autenticada; para maior resistência a manipulação de relógio será necessário desenho adicional. A validação de TLS não deve ser desabilitada para contornar problemas de rede/tempo.

## Raízes e manutenção

Certificados públicos obtidos em 2026-10-05 de [Let's Encrypt](https://letsencrypt.org/certificates/):

- [ISRG Root X1](https://letsencrypt.org/certs/isrgrootx1.pem): SHA256 `96:BC:EC:06:26:49:76:F3:74:60:77:9A:CF:28:C5:A7:CF:E8:A3:C0:AA:E1:1A:8F:FC:EE:05:C0:BD:DF:08:C6`.
- [ISRG Root X2](https://letsencrypt.org/certs/isrg-root-x2.pem): SHA256 `69:72:9B:8E:15:A8:6E:FC:17:7A:57:AF:B7:17:1D:FC:64:AD:D2:8C:2F:CA:8C:F1:50:7E:34:45:3C:CB:14:70`.

A renovação normal do certificado do servidor não exige regravar o gateway enquanto houver cadeia válida até uma raiz confiável. Mudança de autoridade/cadeia incompatível ou retirada de confiança de uma raiz exige revisar e atualizar o firmware. Não presumir que a raiz elimina a necessidade de manutenção.

## Verificações reproduzíveis

```text
g++ -std=c++11 -Wall -Wextra -pedantic test/test_transport_policy.cpp -o test_transport_policy
./test_transport_policy
node scripts/verify-gateway-tls.mjs
```

No Windows, acrescente `.exe` ao teste nativo. O script Node usa exatamente o bundle de raízes do firmware, confere fingerprints e faz handshake TLS1.2 com o broker, recusando nome incorreto e CA não confiável. Não usa credenciais nem publica telemetria. A pilha TLS do Node não substitui o ensaio da biblioteca embarcada.

## Validação em placa ainda necessária

- Wi-Fi → SNTP → MQTT autenticado e reconexão após reinício.
- Sem NTP: MQTT não conecta; ao restaurar NTP, retoma sem reiniciar.
- Senha inválida, hostname errado e CA incorreta: conexão recusada, sem fallback.
- Brinco cadastrado → LoRa → gateway → broker → banco → mapa, com posição real.
- Registrar placa, commit, ambiente, tempo até conexão e eventual perda durante handshake.

Não encerra [resiliência #12](https://github.com/MateussGont/cattle-tracker-lora/issues/12), [ensaio ponta a ponta #9](https://github.com/MateussGont/cattle-tracker-lora/issues/9) ou [identidade do gateway web #14](https://github.com/MateussGont/cattle-tracker-web/issues/14). O gateway continua sem acesso direto ao banco e sem depender da VPN administrativa.
