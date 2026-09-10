# Planejamento semanal — piloto até fevereiro de 2027

## Compromisso e capacidade

Meta confirmada: **piloto validado até 28/02/2027**, com **5 horas semanais no total da equipe**. Início planejado: **14/09/2026**, primeira segunda-feira completa após a definição do prazo. São **24 semanas × 5 h = 120 horas-pessoa**. A organização já entregue até 10/09 não foi debitada novamente deste orçamento.

O plano compromete **105 h em 21 semanas de trabalho** e protege **15 h em três semanas de reserva** (S15, S16 e S23). As semanas de Natal/Ano-Novo continuam dentro do teto, mas não recebem entregas novas: se a equipe não trabalhar nelas, consomem 10 h da margem de disponibilidade e restam 5 h de reserva. Não redistribuir automaticamente essas horas para criar semanas acima de 5 h.

Padrão de uma semana de trabalho: **3 h de execução/preparo + 1,5 h de testes/revisão + 0,5 h de registro/replanejamento = 5 h**. Em semanas de ensaio, redistribuir internamente entre preparação, inspeções e análise. Instalação/preparo do ambiente, leitura, reuniões, montagem, retrabalho, deslocamento, recarga e suporte entram nesse mesmo teto. Duas pessoas em uma reunião de 30 minutos consomem 1 hora-pessoa.

Ensaios de 2 h/24 h medem tempo decorrido. Só o trecho realmente automatizado e sem dedicação humana fica fora do esforço ativo; qualquer supervisão necessária entra nas 5 h. O plano não pressupõe horas extras ou alguém trabalhando gratuitamente fora da conta.

## Entrega de fevereiro e limites

Para caber nesse orçamento, o alvo de planejamento é **cinco brincos, um gateway, uma propriedade e um ambiente restrito ao piloto**, usando módulos e invólucro existentes. A unidade física e o animal ficam associados de forma fixa durante os ensaios. O resultado central é cadastrar, configurar, receber e rastrear dados com identidade e horários corretos, medir confiabilidade e registrar o desempenho físico.

O piloto terá **duas rodadas em dias distintos**, com **pelo menos 4 h de coleta por unidade em cada rodada**, após 24 h de bancada. O Gate C confirma a duração, supervisão e rotina de energia compatíveis com o hardware e as 5 h de trabalho ativo. Se quatro horas de coleta exigirem quatro horas de supervisão presencial, isso será debitado integralmente; deslocamentos e demais tarefas podem exigir replanejar o escopo ou o prazo. Não prometer operação contínua por semanas sem medir autonomia.

Esta é uma previsão com orçamento fixo, não evidência de que todos os defeitos cabem nas estimativas. A montagem, os dois builds e os fluxos integrados ainda precisam ser reproduzidos. A margem é pequena para retrabalho de hardware: validar a premissa de módulos/invólucro utilizáveis na S01. Se o protótipo não for adequado ao animal, o trabalho continua em bancada até aprovação física; não trocar a palavra bancada por piloto validado para cumprir a data.

Ficam fora do compromisso de fevereiro: expansão para 10–25 unidades, vários gateways, PCB própria, desenho industrial, certificação de produto completo, autonomia comercial, redesign amplo, operação multi-instância e preparação integral de produção. Alertas/geofences, histórico completo de trocas de animal, paginação em larga escala e provisionamento em lote não são critérios deste piloto; suas limitações devem aparecer no relatório e não podem ser apresentadas como validadas. Requisitos físicos e aplicáveis ao hardware necessário ao ensaio continuam sendo bloqueadores de entrada em campo.

## Calendário semanal

Cada linha é um bloco de **5 h para a equipe inteira**. Quando há várias issues na mesma linha, elas compartilham as cinco horas; não são cinco horas para cada issue. As datas são a previsão do recorte semanal, não o encerramento automático de uma issue que contenha escopo maior.

| Semana | Período | Horas | Foco e entrega | Referências |
| --- | --- | ---: | --- | --- |
| S01 | 14/09/2026–20/09/2026 | 5 | **Preparar ambiente, materiais e escopo** | [lora #3](https://github.com/MateussGont/cattle-tracker-lora/issues/3), [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7), [lora #8](https://github.com/MateussGont/cattle-tracker-lora/issues/8), [web #20](https://github.com/MateussGont/cattle-tracker-web/issues/20) |
| S02 | 21/09/2026–27/09/2026 | 5 | **Corrigir segurança básica da aplicação** | [web #7](https://github.com/MateussGont/cattle-tracker-web/issues/7), [web #8](https://github.com/MateussGont/cattle-tracker-web/issues/8) |
| S03 | 28/09/2026–04/10/2026 | 5 | **Validar GNSS e ligação elétrica** | [lora #26](https://github.com/MateussGont/cattle-tracker-lora/issues/26), [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7) |
| S04 | 05/10/2026–11/10/2026 | 5 | **Corrigir provisionamento no firmware** | [lora #27](https://github.com/MateussGont/cattle-tracker-lora/issues/27) |
| S05 | 12/10/2026–18/10/2026 | 5 | **Corrigir cadastro USB no aplicativo** | [web #18](https://github.com/MateussGont/cattle-tracker-web/issues/18), [web #4](https://github.com/MateussGont/cattle-tracker-web/issues/4), [lora #25](https://github.com/MateussGont/cattle-tracker-lora/issues/25) |
| S06 | 19/10/2026–25/10/2026 | 5 | **Corrigir propriedade e acesso ao dispositivo** | [web #13](https://github.com/MateussGont/cattle-tracker-web/issues/13), [web #12](https://github.com/MateussGont/cattle-tracker-web/issues/12) |
| S07 | 26/10/2026–01/11/2026 | 5 | **Definir identidade de evento após reinício** | [lora #29](https://github.com/MateussGont/cattle-tracker-lora/issues/29), [web #1](https://github.com/MateussGont/cattle-tracker-web/issues/1) |
| S08 | 02/11/2026–08/11/2026 | 5 | **Corrigir tempo, ordenação e duplicatas** | [web #9](https://github.com/MateussGont/cattle-tracker-web/issues/9), [web #1](https://github.com/MateussGont/cattle-tracker-web/issues/1) |
| S09 | 09/11/2026–15/11/2026 | 5 | **Tornar a ingestão recuperável** | [web #10](https://github.com/MateussGont/cattle-tracker-web/issues/10) |
| S10 | 16/11/2026–22/11/2026 | 5 | **Gate A: validar uma unidade ponta a ponta** | [lora #9](https://github.com/MateussGont/cattle-tracker-lora/issues/9), [web #12](https://github.com/MateussGont/cattle-tracker-web/issues/12) |
| S11 | 23/11/2026–29/11/2026 | 5 | **Separar recepção LoRa e publicação** | [lora #5](https://github.com/MateussGont/cattle-tracker-lora/issues/5) |
| S12 | 30/11/2026–06/12/2026 | 5 | **Recuperar falhas de Wi-Fi e MQTT** | [lora #12](https://github.com/MateussGont/cattle-tracker-lora/issues/12) |
| S13 | 07/12/2026–13/12/2026 | 5 | **Ajustar cadência e preparar métricas** | [lora #28](https://github.com/MateussGont/cattle-tracker-lora/issues/28), [lora #4](https://github.com/MateussGont/cattle-tracker-lora/issues/4), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11) |
| S14 | 14/12/2026–20/12/2026 | 5 | **Montar e identificar cinco unidades** | [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7), [lora #10](https://github.com/MateussGont/cattle-tracker-lora/issues/10), [lora #18](https://github.com/MateussGont/cattle-tracker-lora/issues/18) |
| S15 | 21/12/2026–27/12/2026 | 5 | **Reserva de fim de ano** | Contingência; sem nova funcionalidade |
| S16 | 28/12/2026–03/01/2027 | 5 | **Reserva de fim de ano** | Contingência; sem nova funcionalidade |
| S17 | 04/01/2027–10/01/2027 | 5 | **Gate B: cinco unidades por 24 h em bancada** | [lora #10](https://github.com/MateussGont/cattle-tracker-lora/issues/10), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11), [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7) |
| S18 | 11/01/2027–17/01/2027 | 5 | **Medir autonomia e qualificar o protótipo físico** | [lora #6](https://github.com/MateussGont/cattle-tracker-lora/issues/6), [lora #18](https://github.com/MateussGont/cattle-tracker-lora/issues/18), [lora #21](https://github.com/MateussGont/cattle-tracker-lora/issues/21) |
| S19 | 18/01/2027–24/01/2027 | 5 | **Gate C: autorizar entrada no campo** | [lora #19](https://github.com/MateussGont/cattle-tracker-lora/issues/19), [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22), [lora #18](https://github.com/MateussGont/cattle-tracker-lora/issues/18), [lora #21](https://github.com/MateussGont/cattle-tracker-lora/issues/21) |
| S20 | 25/01/2027–31/01/2027 | 5 | **Piloto de campo — primeira rodada** | [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11) |
| S21 | 01/02/2027–07/02/2027 | 5 | **Piloto de campo — segunda rodada** | [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11) |
| S22 | 08/02/2027–14/02/2027 | 5 | **Corrigir bloqueadores e repetir o cenário afetado** | [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22) |
| S23 | 15/02/2027–21/02/2027 | 5 | **Reserva final de validação** | Contingência; sem nova funcionalidade |
| S24 | 22/02/2027–28/02/2027 | 5 | **Entrega do piloto e decisão de evolução** | [lora #25](https://github.com/MateussGont/cattle-tracker-lora/issues/25), [lora #31](https://github.com/MateussGont/cattle-tracker-lora/issues/31), [web #20](https://github.com/MateussGont/cattle-tracker-web/issues/20) |

## Roteiro e evidência de cada semana

### S01 — Preparar ambiente, materiais e escopo

**14/09/2026 a 20/09/2026 · 5 h**

Reproduzir a execução web e os dois builds de firmware; inventariar uma unidade e os materiais das outras quatro; verificar massa/formato preliminares e disponibilidade de apoio ao manejo. Registrar custos, fornecedores e prazos sem realizar compras automáticas.

**Evidência de saída:** Versões e bloqueios registrados; uma montagem identificada; materiais, local próximo e responsáveis confirmados. Se o ambiente ou a viabilidade física não estiverem encaminhados, reestimar o plano nesta semana.

### S02 — Corrigir segurança básica da aplicação

**21/09/2026 a 27/09/2026 · 5 h**

Corrigir HTML dos popups e limpar/isolar o cache na troca de sessão; executar regressões focadas.

**Evidência de saída:** Marcação no nome do animal não executa código; a troca de conta não exibe dados da sessão anterior.

### S03 — Validar GNSS e ligação elétrica

**28/09/2026 a 04/10/2026 · 5 h**

Conferir a pinagem na montagem real e corrigir leitura/validade temporal do GNSS. Medir alimentação e registrar primeira posição e comportamento sem fixação.

**Evidência de saída:** Posição recente distinguível de leitura antiga; GNSS desconectado não produz fixação falsa; sem reinício por alimentação no ensaio.

### S04 — Corrigir provisionamento no firmware

**05/10/2026 a 11/10/2026 · 5 h**

Implementar parser serial responsivo, confirmação correta da escrita da identidade e tratamento de erro; ensaiar reinício e comandos inválidos.

**Evidência de saída:** GET_STATUS e configuração respondem em até 2 s no modo de serviço; ID confirmado e preservado após reinício.

### S05 — Corrigir cadastro USB no aplicativo

**12/10/2026 a 18/10/2026 · 5 h**

Corrigir identificação das respostas seriais e retomada do cadastro existente; ensaiar o manual com uma unidade.

**Evidência de saída:** Cadastro interrompido é retomado sem criar duplicata; ID do banco confere com a placa depois de reiniciar.

### S06 — Corrigir propriedade e acesso ao dispositivo

**19/10/2026 a 25/10/2026 · 5 h**

Definir e implementar a regra de propriedade do dispositivo, com testes de acesso cruzado. Conferir uma associação por unidade/animal para o inventário do piloto; o histórico completo de trocas continua na issue específica.

**Evidência de saída:** Uma conta fora do escopo não acessa o dispositivo; os cinco vínculos do piloto serão únicos e permanecerão fixos durante o ensaio.

### S07 — Definir identidade de evento após reinício

**26/10/2026 a 01/11/2026 · 5 h**

Implementar identidade de inicialização/sessão e sequência, compatibilidade do pacote e vetores de teste entre transmissor, gateway e API. Recalcular o tamanho do pacote.

**Evidência de saída:** Reinício, versão desconhecida, wrap e duplicata são distinguíveis; um evento novo não é descartado como antigo.

### S08 — Corrigir tempo, ordenação e duplicatas

**02/11/2026 a 08/11/2026 · 5 h**

Separar recebido-em de horário GNSS e implementar deduplicação para um gateway; preservar posição atual diante de mensagens atrasadas.

**Evidência de saída:** Reenvio não duplica a localização; mensagem antiga não faz a posição ou última comunicação retroceder.

### S09 — Tornar a ingestão recuperável

**09/11/2026 a 15/11/2026 · 5 h**

Aplicar transação ao fluxo de persistência e testar falha intermediária, rollback, retry e emissão de eventos após confirmação.

**Evidência de saída:** Falha no banco não deixa estado parcial; reprocessamento preserva uma única localização e um estado consistente.

### S10 — Gate A: validar uma unidade ponta a ponta

**16/11/2026 a 22/11/2026 · 5 h**

Executar pelo menos 2 h contínuas com cadastro, rádio, MQTT, persistência, mapa e histórico; reiniciar placa/gateway/API e observar o resultado. Usar a semana para corrigir os defeitos que bloqueiam esse recorte.

**Evidência de saída:** Logs ligam a unidade física ao animal correto; zero troca de identidade, duplicata lógica ou regressão temporal. Não encerrar issues mais amplas sem cumprir seus demais critérios.

### S11 — Separar recepção LoRa e publicação

**23/11/2026 a 29/11/2026 · 5 h**

Implementar recepção assíncrona, rearme do rádio e fila limitada entre recepção e publicação; testar o tamanho real dos pacotes.

**Evidência de saída:** Lentidão de rede não interrompe o ciclo de recepção; fila e descartes são observáveis.

### S12 — Recuperar falhas de Wi-Fi e MQTT

**30/11/2026 a 06/12/2026 · 5 h**

Dimensionar buffers com tópico/cabeçalho, verificar publish, adicionar reconexão e heartbeat; definir comportamento quando a fila enche.

**Evidência de saída:** Queda e retorno do broker são recuperados; toda perda identificável de fila é contabilizada, sem alegar confirmação de entrega para QoS0.

### S13 — Ajustar cadência e preparar métricas

**07/12/2026 a 13/12/2026 · 5 h**

Experimentar agendamento sem bloqueio e jitter com o pacote final; começar pelo candidato de 60 s e comparar com a bancada. Preparar registro mínimo de TX, RX, banco e motivo de descarte.

**Evidência de saída:** Configuração reproduzível, airtime recalculado e contadores suficientes para medir PDR. Cadência final depende do resultado; não é configuração de produção aprovada.

### S14 — Montar e identificar cinco unidades

**14/12/2026 a 20/12/2026 · 5 h**

Montar/configurar os quatro kits adicionais já adquiridos, identificar as cinco unidades e verificar alimentação, transmissão, massa e fixação preliminares. Usar módulos e invólucro existentes, sem projeto de PCB.

**Evidência de saída:** Cinco IDs únicos, inventário preenchido e unidades prontas para o ensaio prolongado; montagem insegura ou material em falta impede ampliar a escala.

### S15 — Reserva de fim de ano

**21/12/2026 a 27/12/2026 · 5 h**

Capacidade protegida para atrasos de materiais, montagem, correções ou indisponibilidade da equipe. Não assumir funcionalidades novas.

**Evidência de saída:** Registrar se as 5 h foram utilizadas ou indisponíveis e atualizar a reserva restante.

### S16 — Reserva de fim de ano

**28/12/2026 a 03/01/2027 · 5 h**

Segunda janela de contingência para retrabalho ou recesso. Se houver trabalho, escolher o maior bloqueio do Gate B.

**Evidência de saída:** Nenhuma nova obrigação criada por uma reserva não utilizada; disponibilidade real recalculada.

### S17 — Gate B: cinco unidades por 24 h em bancada

**04/01/2027 a 10/01/2027 · 5 h**

Executar coleta automatizada por pelo menos 24 h, induzir falhas previstas e analisar cada unidade. Preparação e análise entram nas 5 h; coleta passiva não substitui supervisão quando ela for necessária.

**Evidência de saída:** PDR por unidade >=95% no cenário declarado, zero identidade trocada/duplicata lógica/corrupção, perdas e recuperação documentadas. TX precisa de contador de referência independente.

### S18 — Medir autonomia e qualificar o protótipo físico

**11/01/2027 a 17/01/2027 · 5 h**

Medir corrente e autonomia, ajustar o perfil dentro da configuração validada e conferir aquecimento, fixação, bordas e proteção. Revisar com responsável de manejo. Registrar limitações; não iniciar desenho industrial.

**Evidência de saída:** Energia suficiente para a janela de campo com margem definida; massa, instalação e remoção aprovadas para este protótipo. Problema físico impede uso em animais.

### S19 — Gate C: autorizar entrada no campo

**18/01/2027 a 24/01/2027 · 5 h**

Validar alcance no local e com o invólucro, conferir requisitos aplicáveis ao hardware e fechar roteiro das duas rodadas: local, duração, recarga, deslocamento, inspeções e critérios de interrupção.

**Evidência de saída:** Local/cobertura, alimentação e condição física aprovados; esforço ativo de operação cabe em 5 h por semana. Havendo falha, usar a capacidade seguinte para corrigir e repetir o gate.

### S20 — Piloto de campo — primeira rodada

**25/01/2027 a 31/01/2027 · 5 h**

Instalar progressivamente uma, depois três e até cinco unidades após conferência; executar coleta e inspeções previstas, mantendo os vínculos fixos. Registrar toda atividade ativa, inclusive deslocamento e recarga.

**Evidência de saída:** Primeira rodada com as cinco unidades identificadas, logs completos e problemas registrados; interromper diante dos critérios definidos no Gate C.

### S21 — Piloto de campo — segunda rodada

**01/02/2027 a 07/02/2027 · 5 h**

Repetir em outro dia/janela, comparar cobertura, fixação GNSS, perdas, disponibilidade e autonomia; coletar relato do operador.

**Evidência de saída:** Duas rodadas comparáveis em dias distintos com cinco unidades, sem troca de identidade nem incidentes físicos. Não inferir autonomia de meses a partir deste ensaio.

### S22 — Corrigir bloqueadores e repetir o cenário afetado

**08/02/2027 a 14/02/2027 · 5 h**

Priorizar somente falhas que afetam os critérios do piloto. Vincular cada correção a uma issue e repetir os testes que podem ter regredido.

**Evidência de saída:** Defeitos críticos resolvidos e cenário reexecutado, ou decisão explícita de reprovação/replanejamento. Adiar aperfeiçoamentos sem impacto no aceite.

### S23 — Reserva final de validação

**15/02/2027 a 21/02/2027 · 5 h**

Proteger 5 h para repetição por clima, conectividade, material ou regressão. Se não for necessária, antecipar relatório, revisão do manual e pacote de entrega.

**Evidência de saída:** Riscos pendentes resolvidos sem introduzir funcionalidades novas na última semana.

### S24 — Entrega do piloto e decisão de evolução

**22/02/2027 a 28/02/2027 · 5 h**

Consolidar relatório, versões/configurações, inventário, manual revisado, evidências e demonstração de restauração do ambiente do piloto. Apresentar resultado e próximos passos.

**Evidência de saída:** Até 28/02: aceite por evidência dos gates e duas rodadas, pendências classificadas e decisão sobre expansão para 10–25 unidades. Só declarar piloto validado se os critérios passarem.

## Gates e decisões

| Data-limite planejada | Gate | Condição para avançar |
| --- | --- | --- |
| 20/09/2026 | Preparação | Ambiente reproduzível ou bloqueios com solução estimada; materiais, montagem viável e apoio físico identificados. |
| 22/11/2026 | A — uma unidade | Cadastro confirmado após reboot e 2 h de fluxo correto, com testes de falha, identidade e tempo. |
| 10/01/2027 | B — cinco unidades | 24 h de bancada com PDR por unidade >=95% no cenário declarado e zero mistura de identidade/duplicação lógica/corrupção. |
| 24/01/2027 | C — entrada no campo | RF, autonomia para a janela, montagem, instalação/remoção e rotina aprovadas; supervisão/deslocamento cabem no orçamento ativo. |
| 07/02/2027 | D — coleta de campo | Duas rodadas de pelo menos 4 h por unidade em dias distintos, cobrindo as cinco unidades e registrando as condições. |
| 28/02/2027 | E — aceite | Relatório e evidências completos, nenhum bloqueador crítico aberto e resultado aprovado por quem responde pelo piloto. |

PDR = pacotes válidos recebidos / tentativas reais de transmissão por unidade. Usar contadores/logs de TX e RX; não derivar tentativas somente de registros que chegaram à API, nem contar frames recebidos por dois gateways como dois eventos. Separar perda de rádio, descarte de fila e falha de ingestão. Sem fonte confiável do denominador, registrar a métrica como inconclusiva e repetir.

Meta inicial de confiabilidade: PDR >=95% por unidade em bancada e nas janelas de cobertura declaradas para o campo; nenhum cruzamento de identidade, corrupção ou duplicata lógica. Registrar latência P50/P95, tempo até fix GNSS, reinícios e autonomia; definir limites de latência/precisão/energia coerentes com a tarefa na S01 e congelá-los antes do Gate A. RSSI/SNR são diagnóstico, não substitutos de PDR. Não mudar o cenário para esconder perdas após observar os resultados.

## Controle de escopo e risco

- **Uma entrega principal em andamento por vez.** O limite anterior de duas implementações simultâneas foi reduzido para caber em cinco horas totais. Não colocar um cartão em execução só porque ele aparece no plano.
- Ao terminar cada semana, registrar horas reais, evidência, maior bloqueio e previsão da próxima. Se uma tarefa ultrapassar a estimativa, terminar o critério essencial e replanejar; não empilhar o atraso sobre as próximas cinco horas.
- Usar reserva somente para bloqueadores do piloto ou indisponibilidade. Se a estimativa restante exceder a capacidade restante, explicitar a diferença em horas e decidir entre mais capacidade, menor escopo de experimento ou mudança de prazo. Nunca remover critérios de integridade ou aprovação física apenas para manter a data.
- **Reavaliar em 22/11 e 10/01.** Se não houver uma unidade estável no Gate A ou cinco unidades prontas no Gate B, o piloto de fevereiro fica em risco; registrar imediatamente a correção do plano.
- Materiais precisam estar disponíveis antes da montagem de dezembro. Aprovação e compra são decisões do responsável; nenhum gasto foi autorizado ou realizado por este planejamento.
- O cronograma é restrito ao piloto. Uma entrega parcial não fecha a issue ampla de 10–25 unidades, PCB, hardware industrial ou produção. Vincular o resultado do recorte e manter os critérios restantes abertos.

## Como acompanhar no GitHub

[Project central](https://github.com/users/MateussGont/projects/1) · [Checklist semanal — issue #32](https://github.com/MateussGont/cattle-tracker-lora/issues/32) · [Marco com prazo em 28/02/2027](https://github.com/MateussGont/cattle-tracker-lora/milestone/5) · [Backlog completo](backlog.md) · [Plano de evolução de longo prazo](plano-evolucao.md).

Este calendário passa a ser a referência para o prazo de fevereiro. As datas antigas das issues do roadmap amplo eram estimativas sem esta restrição de capacidade e não devem ser somadas ao compromisso de 120 h. A issue de planejamento semanal centraliza os 24 blocos, horas e gates, sem criar 24 bugs duplicados nem converter pontos de esforço em horas. O marco de fevereiro registra o aceite do piloto; as fases M1–M4 continuam organizando a evolução mais ampla.
