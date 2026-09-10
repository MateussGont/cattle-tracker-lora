# Backlog e ordem de execução

[Project central](https://github.com/users/MateussGont/projects/1) · [Plano de evolução](plano-evolucao.md) · [Calendário semanal até 28/02/2027](planejamento-semanal.md)

44 issues organizadas entre os dois repositórios: 20 novas e seis transferidas, preservando o histórico das anteriores. Este inventário retrata a organização inicial; o status atualizado fica no GitHub. A prioridade vale dentro da fase: P0 de piloto não significa iniciar antes da bancada.

O planejamento posterior acrescentou a [issue #32 — checklist das 24 semanas](https://github.com/MateussGont/cattle-tracker-lora/issues/32), totalizando 45 itens com a coordenação do prazo. O [marco do piloto](https://github.com/MateussGont/cattle-tracker-lora/milestone/5) vence em 28/02/2027 e centraliza o aceite do recorte dos dois repositórios. As issues M1–M4 preservam seu escopo completo.

## Prioridades iniciais

O calendário semanal é a ordem vigente, com 5 h totais da equipe: S01 prepara ambiente e materiais; os ajustes abaixo se distribuem entre S02 e S08. Esta lista não representa uma única semana de trabalho.

- [ ] [[Segurança] Impedir injeção de HTML nos popups do mapa](https://github.com/MateussGont/cattle-tracker-web/issues/7)
- [ ] [[Segurança] Isolar cache e consultas entre sessões de usuários](https://github.com/MateussGont/cattle-tracker-web/issues/8)
- [ ] [[Brinco] Corrigir pinagem GNSS e validade temporal das leituras](https://github.com/MateussGont/cattle-tracker-lora/issues/26)
- [ ] [[Brinco] Tornar comandos de provisionamento responsivos e persistentes](https://github.com/MateussGont/cattle-tracker-lora/issues/27)
- [ ] [[Provisionamento] Corrigir parser USB e permitir retomada do cadastro](https://github.com/MateussGont/cattle-tracker-web/issues/18)
- [ ] [[Telemetria] Separar horário recebido, horário GNSS e posição atual](https://github.com/MateussGont/cattle-tracker-web/issues/9)

Correção de autorização web#13 está prevista na S06, antes de uso entre contas/propriedades distintas. Identidade/deduplicação ocupa S07–S08; ingestão atômica, S09; validação de uma unidade por 2 h, S10. O recorte do piloto usa associações fixas; alertas e histórico completo de trocas continuam no backlog amplo. Cada semana inclui preparação, execução, validação e registro, sem trabalho paralelo fora do teto da equipe.

## M1

| Prioridade | Repositório / issue | Entrega |
| --- | --- | --- |
| P0 | [lora #3](https://github.com/MateussGont/cattle-tracker-lora/issues/3) | [Brinco] Definir e congelar o escopo mínimo do nó transmissor |
| P0 | [lora #4](https://github.com/MateussGont/cattle-tracker-lora/issues/4) | [Rádio] Dimensionar airtime, colisões e capacidade por gateway |
| P0 | [lora #5](https://github.com/MateussGont/cattle-tracker-lora/issues/5) | [Gateway] Substituir recepção LoRa bloqueante por pipeline assíncrono |
| P0 | [lora #6](https://github.com/MateussGont/cattle-tracker-lora/issues/6) | [Brinco/Hardware] Implementar medição real de bateria e perfil de baixo consumo |
| P0 | [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7) | [Hardware] Montar e validar o protótipo elétrico do brinco em bancada |
| P0 | [lora #9](https://github.com/MateussGont/cattle-tracker-lora/issues/9) | [Sistema] Validar ponta a ponta com 1 brinco e hardware real |
| P0 | [lora #26](https://github.com/MateussGont/cattle-tracker-lora/issues/26) | [Brinco] Corrigir pinagem GNSS e validade temporal das leituras |
| P0 | [lora #27](https://github.com/MateussGont/cattle-tracker-lora/issues/27) | [Brinco] Tornar comandos de provisionamento responsivos e persistentes |
| P0 | [web #7](https://github.com/MateussGont/cattle-tracker-web/issues/7) | [Segurança] Impedir injeção de HTML nos popups do mapa |
| P0 | [web #8](https://github.com/MateussGont/cattle-tracker-web/issues/8) | [Segurança] Isolar cache e consultas entre sessões de usuários |
| P0 | [web #9](https://github.com/MateussGont/cattle-tracker-web/issues/9) | [Telemetria] Separar horário recebido, horário GNSS e posição atual |
| P0 | [web #10](https://github.com/MateussGont/cattle-tracker-web/issues/10) | [Telemetria] Tornar ingestão atômica e recuperável após falha |
| P0 | [web #13](https://github.com/MateussGont/cattle-tracker-web/issues/13) | [Autorização] Definir propriedade do dispositivo e impedir acesso cruzado |
| P0 | [web #18](https://github.com/MateussGont/cattle-tracker-web/issues/18) | [Provisionamento] Corrigir parser USB e permitir retomada do cadastro |
| P1 | [lora #8](https://github.com/MateussGont/cattle-tracker-lora/issues/8) | [QA] Criar CI para protocolo e firmwares com artefatos rastreáveis |
| P1 | [lora #28](https://github.com/MateussGont/cattle-tracker-lora/issues/28) | [Brinco] Implementar agendamento não bloqueante e perfis de rádio mensuráveis |
| P1 | [lora #29](https://github.com/MateussGont/cattle-tracker-lora/issues/29) | [Protocolo] Versionar identidade de evento e contrato entre firmware e web |
| P1 | [lora #30](https://github.com/MateussGont/cattle-tracker-lora/issues/30) | [Arquitetura] Separar aplicação web e estabelecer integração por artefato |
| P1 | [lora #31](https://github.com/MateussGont/cattle-tracker-lora/issues/31) | [Gestão] Acompanhar evolução da bancada ao piloto com gates de validação |
| P1 | [web #11](https://github.com/MateussGont/cattle-tracker-web/issues/11) | [Alertas] Corrigir reconhecimento, recuperação e medições desconhecidas |
| P1 | [web #12](https://github.com/MateussGont/cattle-tracker-web/issues/12) | [Dispositivos] Garantir associação única e histórico correto nas trocas |
| P1 | [web #15](https://github.com/MateussGont/cattle-tracker-web/issues/15) | [Geofencing] Definir áreas permitidas e corrigir alertas entre piquetes |
| P1 | [web #19](https://github.com/MateussGont/cattle-tracker-web/issues/19) | [Regras] Validar a configuração completa após edição parcial |
| P1 | [web #20](https://github.com/MateussGont/cattle-tracker-web/issues/20) | [QA/Release] Validar aplicação independente e distribuir firmware versionado |
| P2 | [lora #2](https://github.com/MateussGont/cattle-tracker-lora/issues/2) | [Produto/Docs] Padronizar o projeto de “colar/coleira” para “brinco” |

## M2

| Prioridade | Repositório / issue | Entrega |
| --- | --- | --- |
| P0 | [lora #10](https://github.com/MateussGont/cattle-tracker-lora/issues/10) | [QA] Montar bancada escalonável com 5, 10 e 25 brincos |
| P0 | [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11) | [QA] Automatizar teste multinós e definir metas de perda, latência e estabilidade |
| P1 | [lora #12](https://github.com/MateussGont/cattle-tracker-lora/issues/12) | [Gateway] Implementar resiliência de Wi‑Fi/MQTT, watchdog e buffer temporário |
| P1 | [lora #17](https://github.com/MateussGont/cattle-tracker-lora/issues/17) | [Segurança] Autenticar mensagens de rádio e planejar gestão de chaves |
| P1 | [web #1](https://github.com/MateussGont/cattle-tracker-web/issues/1) | [Gateway/Backend] Suportar múltiplos gateways e deduplicar recepções |
| P1 | [web #2](https://github.com/MateussGont/cattle-tracker-web/issues/2) | [Backend] Preparar ingestão e retenção para dezenas de brincos |
| P1 | [web #3](https://github.com/MateussGont/cattle-tracker-web/issues/3) | [Aplicativo] Criar painel operacional de brincos e qualidade do enlace |
| P1 | [web #4](https://github.com/MateussGont/cattle-tracker-web/issues/4) | [Aplicativo] Melhorar provisionamento de brincos e gateways |
| P1 | [web #14](https://github.com/MateussGont/cattle-tracker-web/issues/14) | [Segurança] Vincular telemetria à identidade autenticada do gateway |
| P1 | [web #16](https://github.com/MateussGont/cattle-tracker-web/issues/16) | [Tempo real] Reconectar com segurança e atualizar o mapa sem recarga integral |
| P1 | [web #17](https://github.com/MateussGont/cattle-tracker-web/issues/17) | [Aplicativo] Paginar cadastros e exibir histórico e distância do período completo |

## M3

| Prioridade | Repositório / issue | Entrega |
| --- | --- | --- |
| P0 | [lora #18](https://github.com/MateussGont/cattle-tracker-lora/issues/18) | [Hardware/Mecânica] Desenvolver formato físico de brinco seguro para o animal |
| P0 | [lora #19](https://github.com/MateussGont/cattle-tracker-lora/issues/19) | [Hardware/RF] Validar antenas, alcance e conformidade em 915 MHz |
| P0 | [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22) | [Piloto] Planejar e executar piloto de campo com 10–25 brincos |
| P1 | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) | [Hardware] Projetar a PCB integrada do brinco após os gates de validação |
| P1 | [lora #21](https://github.com/MateussGont/cattle-tracker-lora/issues/21) | [Hardware] Validar invólucro e resistência ambiental do brinco |

## M4

| Prioridade | Repositório / issue | Entrega |
| --- | --- | --- |
| P1 | [web #5](https://github.com/MateussGont/cattle-tracker-web/issues/5) | [Aplicativo/UX] Revisar e redesenhar a experiência visual do aplicativo |
| P1 | [web #6](https://github.com/MateussGont/cattle-tracker-web/issues/6) | [Infra] Preparar operação de produção, observabilidade e recuperação |
| P2 | [lora #25](https://github.com/MateussGont/cattle-tracker-lora/issues/25) | [Docs] Criar manuais de montagem, teste e operação por aplicação |

## Regras de manutenção

Responsável ao iniciar; critérios de aceite e evidência antes de fechar. Conferir dependências no corpo da issue. Etiqueta iteration: agora seleciona o primeiro ciclo. Não reproduzir datas antigas como compromissos novos; estimar após conhecer a capacidade e os materiais. A renomeação técnica collar → brinco é P2 e deve manter a compatibilidade entre repositórios.
