# Revisão completa do firmware e configurações propostas
Base analisada: cattle-tracker-lora@4a85dff8. Escopo: platformio.ini, firmware/common/protocol.h, firmware/collar/main.cpp, firmware/receiver/main.cpp, mqtt_publisher.h/.cpp, secrets.h.example, test/test_protocol.cpp e geração do binário. O nome collar é legado. A revisão é estática, com teste nativo do protocolo e cálculo de rádio; não houve compilação PlatformIO ou ensaio físico nesta máquina.

## Parecer
O protocolo compacto e a separação entre transmissor e receptor são apropriados para o MVP. Os maiores ganhos vêm de remover bloqueios, tratar corretamente a idade do GNSS, garantir identidade após reinício, registrar perdas e adotar cadência configurável. Não alterar potência/SF/TCXO sem identificar a revisão física e medir o efeito.

## Configuração observada
| Item | Brinco | Gateway |
|---|---|---|
| Placa | seeed_xiao_esp32s3 + SX1262 | heltec_wifi_lora_32_V2 + SX1276 |
| SPI SCK/MISO/MOSI | 7 / 8 / 9 | 5 / 19 / 27 |
| NSS/reset | 41 / 42 | 18 / 14 |
| IRQ/busy | DIO1=39; busy=40 | DIO0=26; DIO1=35 |
| RF switch externo | GPIO38, LOW durante TX | Sem controle externo no código |
| GNSS | RX=43/TX=44; 9600 baud | Não se aplica |
| LoRa | 915 MHz, BW125 kHz, SF7, CR4/5, preâmbulo 8 | Mesmos valores |
| Potência configurada | 14 dBm | 14 dBm, embora atualmente não transmita |
| TCXO/regulador | 1,8 V, useRegulatorLDO=false | Último argumento gain=0 |
| Periodicidade | janela GNSS 2,5 s + TX + delay 10 s | recepção bloqueante com timeout 2 s |
| Serial | 115200; comandos por linha | 115200; logs JSON |
| MQTT | Não usa | Wi-Fi TCP, PubSubClient, envio sem retenção; retry 5 s |
| Bateria/sono | batteryMv=0; flag inválida; sem sono | Alimentação contínua prevista |

As assinaturas e comportamentos de recepção foram conferidos nas fontes oficiais da [RadioLib 7.7.1 SX1262](https://raw.githubusercontent.com/jgromes/RadioLib/7.7.1/src/modules/SX126x/SX1262.h) e [SX127x](https://raw.githubusercontent.com/jgromes/RadioLib/7.7.1/src/modules/SX127x/SX127x.cpp). A biblioteca aceita recepção assíncrona; o código do projeto escolheu a chamada bloqueante.

## Achados por componente
| Referência | Achado e efeito | Ação indicada |
|---|---|---|
| collar/main.cpp:23–24 | RX/TX contradizem README e comentários: o README liga TX GNSS ao GPIO44, mas firmware escuta GPIO43. | Alinhar pinagem ao circuito; testar sentenças NMEA. |
| collar/main.cpp:72–79,219 | UART é drenada só numa janela de 2,5 s; os 10 s seguintes acumulam dados e podem causar overflow/dados antigos marcados como recém-processados. | Ler continuamente com loop cooperativo; separar aquisição de agendamento TX; contar bytes/checksums. |
| collar/main.cpp:59–69 | Data/hora só usam isValid, sem age; último horário pode continuar válido depois de desconectar o módulo. | Exigir idade e faixa coerentes, marcar tempo desconhecido e separar receivedAt no backend. |
| collar/main.cpp:85 | Validade do GPS não inclui política de qualidade/HDOP/satélites; isValid não significa precisão suficiente. | Registrar idade e diagnóstico; definir limiar a partir de medições, sem inventar precisão em metros a partir de HDOP. |
| collar/main.cpp:11,219 | “10 s” é pausa após trabalho; período real supera 12,5 s. Transmissores podem manter colisões se ligados juntos. | Agendar pelo relógio monotônico, introduzir fase inicial aleatória e jitter; perfis explícitos. |
| collar/main.cpp:121–149 | readStringUntil pode bloquear; sscanf aceita texto residual; não há limite de linha controlado. | Parser incremental com limite, sintaxe integral, prazo e resposta correlacionada. |
| collar/main.cpp:142,159 | Retorno de begin/putUShort de Preferences é ignorado; sucesso pode ser anunciado sem persistência. | Só confirmar após escrita/leitura; erro específico; testar falha e reboot. |
| collar/main.cpp:41,83 | Sequência reinicia em zero; sem bootId/identidade de evento. | Versionar protocolo com sessão de inicialização; não gravar contador em flash a cada TX. |
| collar/main.cpp:103–107 | Erro de rádio trava permanentemente inclusive provisionamento. | Estado de falha diagnosticável, tentativas limitadas/backoff; serviço USB permanece acessível. |
| collar/main.cpp:208–219 | Erro de TX só é impresso; não há contadores de tentativas/falhas. | Métricas persistentes por sessão, códigos de erro e política limitada de recuperação. |
| collar/main.cpp:98–99 | Sem medição e sem gerenciamento de energia. | Medir corrente real antes de escolher sono; desligar periféricos compatíveis e medir custo de reacquirir GNSS. |
| receiver/main.cpp:59–94 | Recepção e rede ocupam o mesmo fluxo; reconexão TCP/MQTT pode bloquear. | ISR apenas sinaliza; tarefa do rádio lê/valida/enfileira e rearma RX; tarefa de rede consome fila. |
| receiver/main.cpp:67–75 | Comprimento fornecido ao decoder é sempre 26, não explicitamente o comprimento efetivo recebido. | Consultar tamanho do pacote e rejeitar tamanho divergente antes de decodificar; ensaiar payload curto/longo. |
| receiver/main.cpp:68–70 | Timeout normal de rádio vira rx_error a cada 2 s. | Separar ausência esperada de pacote de falhas; agregar contadores e limitar logs. |
| mqtt_publisher.cpp:64–83 | Sem fila/reenvio; retorno falso significa descarte. QoS do subscriber não altera QoS do publish. | Fila limitada, idade/overflow explícitos, reenvio idempotente, contagem de perda e confirmação adequada. |
| mqtt_publisher.cpp:81–83 | Buffer JSON e buffer MQTT confundidos; tamanho total pode exceder 256. | measureJson, limites para configuração, buffer MQTT dimensionado e falhas visíveis. |
| mqtt_publisher.cpp:46–61 | Retry fixo, sem jitter, sem limitar tempo bloqueado da biblioteca. | Backoff e tarefas separadas; configurar timeouts a partir de teste e contabilizar indisponibilidade. |
| mqtt_publisher.cpp + backend | Gateway não emite heartbeat próprio; presença é inferida da telemetria de brincos conhecidos. | Heartbeat com uptime/versão/saúde/fila; independente de GNSS e presença de animais. |
| secrets.h.example/mqtt_publisher.h | Credenciais embutidas e WiFiClient sem TLS. | Identidade individual, gestão de segredos e transporte autenticado; nunca publicar binário do gateway contendo segredos. |
| protocol.h | CRC e magic/version são úteis; faltam bootId, autenticação e defesa contra replay. | Planejar v2 e compatibilidade coordenada; CRC não substitui autenticação. |
| platformio.ini | Plataforma sem versão fixada; bibliotecas com faixas; builds futuros podem variar. | Após validar toolchain, registrar versões exatas e artefatos/revisões. Não alterar dependências às cegas. |
| sync-firmware.mjs | Geração estava acoplada ao frontend. | Exportação independente com SHA-256 e importação explícita no repositório web. |

Os campos valid/age e a atualização das sentenças foram verificados na implementação da [TinyGPSPlus 1.0.3](https://raw.githubusercontent.com/mikalhart/TinyGPSPlus/v1.0.3/src/TinyGPS%2B%2B.cpp) e [tipos da biblioteca](https://raw.githubusercontent.com/mikalhart/TinyGPSPlus/v1.0.3/src/TinyGPS%2B%2B.h). A [pinagem oficial XIAO](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#hardware-overview) confirma D7/GPIO44 e D6/GPIO43.

O orçamento MQTT deve incluir cabeçalho e tópico: um exemplo válido de JSON com 236 bytes e gatewayId de 64 caracteres exige 267 bytes no publish atual; o buffer padrão de 256 não basta. Ver [PubSubClient 2.8 .h](https://raw.githubusercontent.com/knolleary/pubsubclient/v2.8/src/PubSubClient.h) e [.cpp](https://raw.githubusercontent.com/knolleary/pubsubclient/v2.8/src/PubSubClient.cpp).

## Verificações físicas pendentes
Identificar revisão do Wio (B2B versus pinos), TCXO e chave RF. O código controla GPIO38 manualmente; validar a tabela verdade no esquema da revisão utilizada e medir TX/RX antes de trocar níveis. Comparar o projeto com o [kit Seeed](https://wiki.seeedstudio.com/wio_sx1262_with_xiao_esp32s3_kit/) e seu [esquema](https://files.seeedstudio.com/products/SenseCAP/Wio_SX1262/Schematic_Diagram_Wio-SX1262_for_XIAO.pdf). Não foi possível atestar a montagem física pela revisão do repositório.

Conferir modelo GNSS, baud real, sentenças emitidas, alimentação, antena, boot USB, estabilidade sob pico de TX e integridade da gravação NVS. A placa Heltec V2 não deve ser substituída por V3/V4 sem revisar pinos, rádio e driver.

## Perfis experimentais propostos
| Parâmetro | Bancada de um nó | Experimento com 5–25 nós | Experimento de autonomia |
|---|---|---|---|
| Frequência/BW/SF/CR | Preservar 915/125/SF7/4-5 | Mesmo PHY para comparação | Igual ao perfil de campo validado |
| Intervalo entre tentativas | 10 s agendados | 60 s inicialmente; comparar 30 s | Comparar 60/300 s |
| Fase e jitter | Fase aleatória; jitter opcional de teste | Fase uniforme [0,T), jitter ±10% como hipótese inicial | Mesmo princípio |
| Preâmbulo/header/CRC/IQ | 8/explícito/CRC ativo/IQ normal, configurados explicitamente nos dois lados | Idênticos | Idênticos |
| Potência | 14 dBm como referência do código | Ajustar somente com margem medida | Menor potência que atenda cobertura medida |
| GNSS | Leitura contínua, qualidade/idade visíveis | Separar leitura da cadência TX | Comparar manter ligado versus desligar, incluindo TTFF |
| Gateway | RX assíncrono sempre ativo | RX separado da rede + fila | Igual |
| Modo de serviço USB | Sem sono, resposta alvo ≤2 s | Acesso local à configuração | Janela/botão para suspender sono |

Esses perfis não foram aplicados nem aprovados em campo. 915 MHz é a frequência atual do código, não uma certificação de conformidade; a issue de RF exige revisão das condições aplicáveis antes do piloto.

## Airtime calculado
Modelo: payload 26 bytes, BW125, CR4/5, header explícito, CRC físico ativo, preâmbulo 8; LDRO para símbolos longos. Script reproduzível: node scripts/radio-budget.mjs. Os cálculos seguem o modelo LoRa; compare com [Semtech LoRa Calculator](https://www.semtech.com/design-support/lora-calculator) e o getTimeOnAir da RadioLib na configuração real.

| SF | Airtime estimado | Carga oferecida de 25 nós a 60 s |
|---|---:|---:|
| 7 | 61,696 ms | 2,571% |
| 8 | 113,152 ms | 4,715% |
| 9 | 205,824 ms | 8,576% |
| 10 | 411,648 ms | 17,152% |
| 11 | 823,296 ms | 34,304% |
| 12 | 1646,592 ms | 68,608% |

Aumentar SF7→SF12 multiplica o tempo de rádio em aproximadamente 26,7 neste exemplo. Portanto, não usar SF alto como resposta automática a alcance insuficiente. Melhorar antena/posição do gateway, reduzir cadência e medir a margem.

Para 25 nós SF7 a 12,5 s, G=N×ToA/T≈0,1234; a 60 s, G≈0,0257. O modelo ideal de ALOHA puro P≈exp(-2G) sugere aproximadamente 78,1% e 95,0% de sucesso, respectivamente. Isso assume chegadas aleatórias independentes, mesmo canal/SF, sem capture, interferência ou retransmissão; não prevê a PDR real de transmissores periódicos sincronizados. Jitter reduz correlação, mas não garante a meta.

## Arquitetura interna proposta
Brinco: estados Boot → Não provisionado/Serviço → Aquisição → Pronto para TX → TX → Espera/Sono, com estado de falha recuperável. Leitura serial/GNSS não deve esperar o próximo ciclo completo. Configuração de produto separada da pinagem da placa.

Gateway: interrupção registra sinal, tarefa de rádio acessa SPI e reativa recepção, fila contém pacote e RSSI/SNR/tempo, tarefa de rede publica. Nunca acessar rede, fazer logging pesado ou serializar JSON na ISR. Definir proteção concorrente e limites de memória; nenhum crescimento ilimitado de fila.

Contrato v2: negociar boot/session ID, sequência, versão e autenticação com backend antes de alterar tamanho binário. Timestamp do gateway ajuda diagnóstico, mas receivedAt do servidor permanece uma medida distinta. Guardar evento lógico e observações por gateway.

## Plano de ensaios
1. Host: vetor de referência, limites de tamanho, pointers nulos, endian, valores extremos, CRC conhecido e bit flips.
2. USB: comandos fragmentados/agrupados, eventos espontâneos, linha inválida/longa, perda de alimentação após gravação e duas operações concorrentes.
3. GNSS: sem módulo, sem fixação, fix perdido, horário antigo/futuro e fluxo UART contínuo.
4. Rádio: parâmetros explicitamente iguais, payload curto/longo, interferência, tentativas simultâneas, potência/antena e reset de receptor.
5. Rede: Wi-Fi/broker offline, fila cheia, reinício, reconexão e atraso; correlacionar cada tentativa até a persistência.
6. Energia: corrente medida em cada estado, NVS/bootId após sono e autonomia com temperatura/capacidade representativas.
7. Soak: 2 h com um nó e 24 h por perfil candidato multinós; registrar commits, versões, topologia e resultados.

O teste nativo original passou; as recomendações de pinagem/loop/RF ainda precisam de implementação e validação em placa. As otimizações desta entrega são a separação do processo de geração de artefatos e o modelo reproduzível de capacidade, sem alteração silenciosa da configuração de rádio.
