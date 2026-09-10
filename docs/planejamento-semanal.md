# Planejamento semanal — PCB v1 e piloto até fevereiro de 2027

## Compromisso e capacidade

**Meta confirmada: PCB v1 do brinco fabricada, montada e testada, junto com piloto validado até 28/02/2027. Capacidade: 6 horas semanais no total da equipe.** Esta revisão substitui o plano anterior de 5 h que deixava a PCB para depois.

De **14/09/2026 a 28/02/2027** são **24 semanas × 6 h = 144 horas-pessoa**: **126 h em 21 semanas de entregas + 18 h em três semanas de reserva** (S15, S16 e S23). A organização já publicada não é debitada novamente. As reservas de Natal/Ano-Novo são também margem de disponibilidade: se forem recesso, restam **132 h disponíveis e 6 h de reserva**. Não exceder 6 h em outra semana para compensar.

| Alocação principal | Semanas | Horas |
| --- | --- | ---: |
| Preparação | S01 | 6 |
| Firmware, web e integração | S02–S07, S13–S14, S18 | 54 |
| Energia, RF e mecânica antes da PCB | S08–S09 | 12 |
| Esquema, layout, revisão/fabricação e montagem | S10–S12, S17 | 24 |
| Bancada do lote e aprovação para campo | S19–S20 | 12 |
| Duas rodadas de campo | S21–S22 | 12 |
| Documentação e aceite | S24 | 6 |
| Reservas | S15–S16, S23 | 18 |
| **Total** | **24 semanas** | **144** |

Padrão semanal: **4 h de execução/preparo + 1,5 h de validação/revisão + 0,5 h de registro = 6 h**. Nas semanas com orçamento interno específico e nos ensaios, redistribuir dentro desse teto. Instalação de ambiente, aprendizado, leitura, reuniões, revisões de terceiros da equipe, montagem, acompanhamento de fornecedor, deslocamentos, recargas e suporte contam. Duas pessoas em reunião de 30 minutos consomem 1 hora-pessoa.

As horas são uma alocação de capacidade, não uma estimativa comprovada de conclusão. A PCB parte de fontes CAD ainda não encontradas no repositório. A previsão exige executor com experiência, revisor disponível, montagem simples e módulos já conhecidos. Validar as estimativas na S01; esforço externo contratado também deve ser explicitado, sem presumir ajuda fora da conta.

## O que será entregue

Alvo: **cinco brincos com PCB v1 própria, um gateway e uma propriedade**, em ambiente restrito ao piloto. Para esta v1, adota-se como premissa de planejamento uma **placa de integração dos módulos existentes** de processamento, LoRa e GNSS, com alimentação/proteção, medição de bateria, conectores e pontos de teste. A seleção exata será congelada após os ensaios. Não pressupõe projeto RF com chips discretos nem miniaturização final.

PCB pronta significa placa física funcional e testada, com fontes editáveis e pacote de fabricação, conforme o [plano e aceite da PCB v1](plano-pcb-v1.md). Apenas esquema, renderização, Gerbers ou placa sem teste não satisfazem a entrega. O piloto deverá usar a v1; o protótipo modular serve à redução de risco antes da fabricação.

As cinco unidades passam por **24 h de bancada**, depois **duas rodadas de campo em dias distintos, com pelo menos 4 h de coleta por unidade em cada rodada**. Supervisão necessária é esforço ativo; somente a coleta automatizada sem dedicação fica fora das horas-pessoa. Deslocamento, manejo e inspeções devem caber nas 6 h de cada semana de campo. Associações entre animal e unidade permanecem fixas.

Ficam para a evolução posterior: 10–25 unidades, vários gateways, PCB do gateway, RF discreto, desenho industrial final, certificação completa de produto, autonomia comercial, redesign amplo, operação multi-instância e produção completa. Alertas/geofences, histórico completo de troca de animais, paginação em grande escala e provisionamento em lote permanecem fora do aceite. Os requisitos físicos e os requisitos aplicáveis ao hardware utilizado no ensaio continuam obrigatórios.

## Calendário semanal

Cada linha soma **6 h da equipe inteira**, compartilhadas entre todas as referências. Uma entrega parcial não encerra uma issue cujo escopo seja maior. Uma entrega principal em andamento por vez.

| Semana | Período | Horas | Foco e entrega | Referências |
| --- | --- | ---: | --- | --- |
| S01 | 14/09/2026–20/09/2026 | 6 | **Preparar ambiente e viabilidade da PCB v1** | [lora #3](https://github.com/MateussGont/cattle-tracker-lora/issues/3), [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7), [lora #8](https://github.com/MateussGont/cattle-tracker-lora/issues/8), [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20), [web #20](https://github.com/MateussGont/cattle-tracker-web/issues/20) |
| S02 | 21/09/2026–27/09/2026 | 6 | **Corrigir GNSS e provisionamento do firmware** | [lora #26](https://github.com/MateussGont/cattle-tracker-lora/issues/26), [lora #27](https://github.com/MateussGont/cattle-tracker-lora/issues/27), [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7) |
| S03 | 28/09/2026–04/10/2026 | 6 | **Fechar segurança e propriedade do dispositivo** | [web #7](https://github.com/MateussGont/cattle-tracker-web/issues/7), [web #8](https://github.com/MateussGont/cattle-tracker-web/issues/8), [web #13](https://github.com/MateussGont/cattle-tracker-web/issues/13), [web #12](https://github.com/MateussGont/cattle-tracker-web/issues/12) |
| S04 | 05/10/2026–11/10/2026 | 6 | **Validar cadastro USB no aplicativo** | [web #18](https://github.com/MateussGont/cattle-tracker-web/issues/18), [web #4](https://github.com/MateussGont/cattle-tracker-web/issues/4), [lora #25](https://github.com/MateussGont/cattle-tracker-lora/issues/25) |
| S05 | 12/10/2026–18/10/2026 | 6 | **Implementar identidade de evento e contrato** | [lora #29](https://github.com/MateussGont/cattle-tracker-lora/issues/29), [web #1](https://github.com/MateussGont/cattle-tracker-web/issues/1) |
| S06 | 19/10/2026–25/10/2026 | 6 | **Corrigir horários, ordenação e duplicatas** | [web #9](https://github.com/MateussGont/cattle-tracker-web/issues/9), [web #1](https://github.com/MateussGont/cattle-tracker-web/issues/1) |
| S07 | 26/10/2026–01/11/2026 | 6 | **Ingestão recuperável e Gate A com uma unidade** | [web #10](https://github.com/MateussGont/cattle-tracker-web/issues/10), [lora #9](https://github.com/MateussGont/cattle-tracker-lora/issues/9), [web #12](https://github.com/MateussGont/cattle-tracker-web/issues/12) |
| S08 | 02/11/2026–08/11/2026 | 6 | **Qualificar alimentação e medição de bateria** | [lora #6](https://github.com/MateussGont/cattle-tracker-lora/issues/6), [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7), [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S09 | 09/11/2026–15/11/2026 | 6 | **Congelar envelope mecânico, antena e interfaces** | [lora #18](https://github.com/MateussGont/cattle-tracker-lora/issues/18), [lora #19](https://github.com/MateussGont/cattle-tracker-lora/issues/19), [lora #21](https://github.com/MateussGont/cattle-tracker-lora/issues/21), [lora #4](https://github.com/MateussGont/cattle-tracker-lora/issues/4), [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S10 | 16/11/2026–22/11/2026 | 6 | **Desenhar esquema e lista de materiais da PCB v1** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S11 | 23/11/2026–29/11/2026 | 6 | **Roteamento e revisão do layout v1** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20), [lora #18](https://github.com/MateussGont/cattle-tracker-lora/issues/18), [lora #19](https://github.com/MateussGont/cattle-tracker-lora/issues/19) |
| S12 | 30/11/2026–06/12/2026 | 6 | **Liberar fabricação após revisão independente** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S13 | 07/12/2026–13/12/2026 | 6 | **Separar recepção LoRa e publicação** | [lora #5](https://github.com/MateussGont/cattle-tracker-lora/issues/5) |
| S14 | 14/12/2026–20/12/2026 | 6 | **Recuperar falhas de Wi-Fi e MQTT** | [lora #12](https://github.com/MateussGont/cattle-tracker-lora/issues/12) |
| S15 | 21/12/2026–27/12/2026 | 6 | **Reserva de fim de ano** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S16 | 28/12/2026–03/01/2027 | 6 | **Reserva de fim de ano** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S17 | 04/01/2027–10/01/2027 | 6 | **Montar lote v1 e executar testes elétricos iniciais** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20), [lora #7](https://github.com/MateussGont/cattle-tracker-lora/issues/7) |
| S18 | 11/01/2027–17/01/2027 | 6 | **Integrar firmware na v1 e medir perfil final** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20), [lora #28](https://github.com/MateussGont/cattle-tracker-lora/issues/28), [lora #6](https://github.com/MateussGont/cattle-tracker-lora/issues/6), [lora #4](https://github.com/MateussGont/cattle-tracker-lora/issues/4), [lora #9](https://github.com/MateussGont/cattle-tracker-lora/issues/9), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11) |
| S19 | 18/01/2027–24/01/2027 | 6 | **Gate B: cinco PCBs v1 por 24 h em bancada** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20), [lora #10](https://github.com/MateussGont/cattle-tracker-lora/issues/10), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11) |
| S20 | 25/01/2027–31/01/2027 | 6 | **Gate C: qualificar a montagem final para campo** | [lora #18](https://github.com/MateussGont/cattle-tracker-lora/issues/18), [lora #19](https://github.com/MateussGont/cattle-tracker-lora/issues/19), [lora #21](https://github.com/MateussGont/cattle-tracker-lora/issues/21), [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22) |
| S21 | 01/02/2027–07/02/2027 | 6 | **Piloto com PCB v1 — primeira rodada** | [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11), [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S22 | 08/02/2027–14/02/2027 | 6 | **Piloto com PCB v1 — segunda rodada** | [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22), [lora #11](https://github.com/MateussGont/cattle-tracker-lora/issues/11), [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) |
| S23 | 15/02/2027–21/02/2027 | 6 | **Reserva final: corrigir e repetir testes** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20), [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22) |
| S24 | 22/02/2027–28/02/2027 | 6 | **Entregar PCB v1 documentada e piloto validado** | [lora #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20), [lora #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22), [lora #25](https://github.com/MateussGont/cattle-tracker-lora/issues/25), [lora #31](https://github.com/MateussGont/cattle-tracker-lora/issues/31), [web #20](https://github.com/MateussGont/cattle-tracker-web/issues/20) |

## Roteiro e evidência de cada semana

### S01 — Preparar ambiente e viabilidade da PCB v1

**14/09/2026 a 20/09/2026 · 6 h**

Reproduzir os dois builds e a aplicação; inventariar módulos, bateria, ferramentas e materiais; confirmar quem desenha e quem revisa a PCB, experiência no CAD, apoio ao manejo e local. Registrar previsão de orçamento e entrega, sem executar compras.

**Evidência de saída:** Ambiente e unidade de referência identificados; estimativas revistas pelo executor; responsável e revisor de hardware disponíveis. Sem essas condições, replanejar agora.

### S02 — Corrigir GNSS e provisionamento do firmware

**21/09/2026 a 27/09/2026 · 6 h**

Conferir pinagem real e validade temporal do GNSS; corrigir atendimento serial e persistência do ID. Restringir a mudança ao necessário para cadastrar e testar uma unidade.

**Evidência de saída:** Fixação antiga/desconexão distinguíveis; GET_STATUS e configuração em até 2 s no modo de serviço; ID preservado após reinício. Divisão prevista: 3 h GNSS + 3 h serial, incluindo testes.

### S03 — Fechar segurança e propriedade do dispositivo

**28/09/2026 a 04/10/2026 · 6 h**

Corrigir popups, isolamento de cache e autorização do dispositivo; conferir vínculos únicos para o piloto.

**Evidência de saída:** Conteúdo do nome não executa código, sessão anterior não vaza e acesso cruzado é negado. Divisão prevista: 3 h popups/cache + 3 h autorização/testes; vínculos ficarão fixos durante o piloto.

### S04 — Validar cadastro USB no aplicativo

**05/10/2026 a 11/10/2026 · 6 h**

Corrigir parser e retomada de cadastro; executar o manual com a unidade de referência e registrar o ID físico/banco.

**Evidência de saída:** Cadastro interrompido retoma sem duplicar; configuração sobrevive ao reinício. Melhorias de lote e redesign do assistente ficam fora do recorte.

### S05 — Implementar identidade de evento e contrato

**12/10/2026 a 18/10/2026 · 6 h**

Implementar identidade por inicialização/sessão e sequência, versão do pacote e vetores comuns entre brinco, gateway e API.

**Evidência de saída:** Reinício, wrap, pacote desconhecido e duplicata têm comportamento testado; tamanho do pacote atualizado. Recorte de um gateway.

### S06 — Corrigir horários, ordenação e duplicatas

**19/10/2026 a 25/10/2026 · 6 h**

Separar recebido-em de horário GNSS, deduplicar e impedir regressão da posição por mensagem atrasada.

**Evidência de saída:** Reenvio não cria nova localização lógica; evento atrasado não faz posição ou última comunicação retroceder.

### S07 — Ingestão recuperável e Gate A com uma unidade

**26/10/2026 a 01/11/2026 · 6 h**

Implementar transação/rollback/reprocessamento e executar pelo menos 2 h de fluxo ponta a ponta na montagem de referência, incluindo reinícios e falhas.

**Evidência de saída:** Cadastro, rádio, MQTT, banco e mapa coerentes, sem identidade trocada/duplicata/corrupção. Reservar 4 h para correção/testes focados e 2 h para preparação/análise do ensaio; coleta passiva pode durar mais.

### S08 — Qualificar alimentação e medição de bateria

**02/11/2026 a 08/11/2026 · 6 h**

Medir corrente em aquisição, TX e espera; selecionar bateria, proteção, carga e circuito de medição. Ensaiar o circuito de referência que será levado à PCB e registrar calibração.

**Evidência de saída:** Alimentação e picos medidos; tensão real com erro conhecido; limites e capacidade para a janela de campo definidos. Sono comercial e comparação ampla de baterias permanecem na evolução; mudanças indispensáveis à janela são bloqueadoras.

### S09 — Congelar envelope mecânico, antena e interfaces

**09/11/2026 a 15/11/2026 · 6 h**

Ensaiar antena no invólucro representativo e alimentação escolhida; verificar massa/formato/fixação com apoio de manejo. Recalcular airtime do pacote atual e congelar dimensões/conectores. Confirmar fornecedor, orçamento, revisão e janela de recebimento.

**Evidência de saída:** Gate H: interfaces e montagem elétrica estáveis; energia, formato e RF adequados à v1. Escopo é o protótipo do piloto. Não iniciar o layout com pinagem, antena ou envelope indefinidos.

### S10 — Desenhar esquema e lista de materiais da PCB v1

**16/11/2026 a 22/11/2026 · 6 h**

Criar fontes editáveis da placa que integra os módulos existentes, alimentação/proteção/medição, programação, conectores e pontos de teste. Conferir símbolos e footprints reais.

**Evidência de saída:** Esquema e BOM versionados; verificação elétrica executada; toda exceção justificada e nenhum erro crítico pendente.

### S11 — Roteamento e revisão do layout v1

**23/11/2026 a 29/11/2026 · 6 h**

Roteiar a placa dentro do envelope aprovado; conferir retorno de corrente, regras RF, áreas livres de antena, acesso à programação e teste. Revisar encaixe e dimensões em escala real.

**Evidência de saída:** Layout coerente com o esquema, regras do fabricante configuradas, verificação de layout e conferência mecânica registradas. Revisão começa nesta semana, sem aguardar o último dia.

### S12 — Liberar fabricação após revisão independente

**30/11/2026 a 06/12/2026 · 6 h**

Concluir revisão independente, corrigir pendências, inspecionar Gerbers/furação e pacote de montagem. Responsável aprova orçamento e encomenda lote suficiente para obter cinco placas funcionais.

**Evidência de saída:** Gate F até 06/12: revisão aprovada e pedido aceito pelo fabricante, com previsão de recebimento antes da montagem de 04/01. Somente arquivos exportados não contam como fabricação iniciada. As 6 h incluem revisão, ajustes e acompanhamento ativo.

### S13 — Separar recepção LoRa e publicação

**07/12/2026 a 13/12/2026 · 6 h**

Implementar recepção assíncrona, rearme e fila limitada; testar rede lenta com módulos de referência enquanto a PCB é fabricada.

**Evidência de saída:** Recepção continua com publicação lenta; limite, ocupação e descartes da fila observáveis. Espera do fabricante não consome dedicação, mas acompanhamento consome.

### S14 — Recuperar falhas de Wi-Fi e MQTT

**14/12/2026 a 20/12/2026 · 6 h**

Dimensionar buffer com tópico/cabeçalho, tratar publish, reconexão, heartbeat e fila cheia; registrar TX/RX e contadores básicos para o ensaio.

**Evidência de saída:** Broker/rede recuperam após falha e perdas identificáveis são contabilizadas. Não alegar entrega confirmada para QoS0. Conferir andamento do pedido dentro das 6 h.

### S15 — Reserva de fim de ano

**21/12/2026 a 27/12/2026 · 6 h**

Janela protegida para atraso, recesso ou ajustes de fabricação. Nenhuma funcionalidade nova.

**Evidência de saída:** Registrar horas usadas ou indisponíveis; atualizar previsão do fabricante e reserva restante.

### S16 — Reserva de fim de ano

**28/12/2026 a 03/01/2027 · 6 h**

Segunda janela protegida de fim de ano. Confirmar recebimento da PCB e materiais para a montagem de janeiro.

**Evidência de saída:** Se as placas não chegarem a tempo, registrar impacto no caminho crítico e replanejar; espera não garante entrega.

### S17 — Montar lote v1 e executar testes elétricos iniciais

**04/01/2027 a 10/01/2027 · 6 h**

Inspecionar as placas recebidas; testar uma com alimentação limitada antes de montar o restante; conferir tensões, programação, USB, GNSS, rádio e medição. Montar e identificar cinco unidades funcionais.

**Evidência de saída:** Cinco PCBs v1 montadas, sem falha elétrica crítica, identificadas por revisão/serial e com checklist por unidade. Montagem/retrabalho deve caber no orçamento; processo complexo ou componentes discretos exigem nova estimativa.

### S18 — Integrar firmware na v1 e medir perfil final

**11/01/2027 a 17/01/2027 · 6 h**

Ajustar cadência/jitter com o pacote final; consolidar contadores TX/RX/banco e leitura de bateria; medir consumo na PCB real e repetir pelo menos 2 h ponta a ponta.

**Evidência de saída:** Configuração rastreável, bateria real e autonomia para a janela com margem definida; rádio e identidade corretos na PCB. Usar candidato de 60 s como experimento, com resultado documentado.

### S19 — Gate B: cinco PCBs v1 por 24 h em bancada

**18/01/2027 a 24/01/2027 · 6 h**

Executar pelo menos 24 h de coleta automatizada com as cinco unidades, induzir falhas previstas e comparar TX/RX/banco por unidade.

**Evidência de saída:** Até 24/01: lote v1 montado e testado; PDR >=95% por unidade no cenário declarado; zero identidade trocada, duplicata lógica ou corrupção; perdas e recuperação explicadas.

### S20 — Gate C: qualificar a montagem final para campo

**25/01/2027 a 31/01/2027 · 6 h**

Repetir RF, energia, montagem e inspeções no invólucro com a PCB final. Fechar local, instalação/remoção, recarga, supervisão, deslocamento e critérios de interrupção.

**Evidência de saída:** Até 31/01: protótipo final aprovado para uso no animal e ensaios do piloto; esforço ativo cabe em 6 h semanais. Protótipo modular anterior não substitui essa aprovação da v1.

### S21 — Piloto com PCB v1 — primeira rodada

**01/02/2027 a 07/02/2027 · 6 h**

Instalar progressivamente uma, três e cinco unidades após conferência; executar pelo menos 4 h de coleta por unidade e inspeções previstas.

**Evidência de saída:** Primeira rodada nas cinco PCBs v1 com inventário, configurações, TX/RX e incidentes registrados. Vínculos animal/unidade fixos.

### S22 — Piloto com PCB v1 — segunda rodada

**08/02/2027 a 14/02/2027 · 6 h**

Repetir em dia distinto com pelo menos 4 h de coleta por unidade; comparar cobertura, GNSS, perdas e autonomia; recolher relato do operador.

**Evidência de saída:** Até 14/02: duas rodadas comparáveis nas cinco unidades, PDR >=95% nas janelas declaradas, sem mistura de identidade nem incidente físico.

### S23 — Reserva final: corrigir e repetir testes

**15/02/2027 a 21/02/2027 · 6 h**

Usar até 6 h para bloqueadores e repetição por defeito, clima ou conectividade. Se não necessária, antecipar relatório e pacote de entrega.

**Evidência de saída:** Correção vinculada a evidência e cenário reexecutado. Uma nova fabricação não é presumida possível nesta janela.

### S24 — Entregar PCB v1 documentada e piloto validado

**22/02/2027 a 28/02/2027 · 6 h**

Consolidar CAD, BOM, fabricação, montagem, testes por placa, versões de firmware/web, inventário, manual revisado e relatório de campo. Demonstrar restauração do ambiente do piloto e obter aceite.

**Evidência de saída:** Até 28/02: PCB v1 física funcional e piloto com evidências; nenhum bloqueador crítico aberto; próximos passos da v1.1/v2 documentados. Só declarar validação se os critérios passarem.

## Gates e decisões

| Até | Marco | Evidência necessária |
| --- | --- | --- |
| 20/09/2026 | Preparação | Ambiente, executor/revisor de hardware, materiais e estimativas confirmados. |
| 01/11/2026 | A — uma unidade | Cadastro e 2 h ponta a ponta na referência, com identidade, tempo e recuperação corretos. |
| 15/11/2026 | H — base física | Energia, circuito de medição, envelope, antena e interfaces aprovados para projetar a v1. |
| 06/12/2026 | F — fabricação | Esquema/layout revisados e pedido aceito, com recebimento planejado antes de 04/01. |
| 10/01/2027 | Montagem | Cinco placas v1 montadas e aprovadas nos testes elétricos iniciais. |
| 24/01/2027 | B — PCB testada | Cinco PCBs por 24 h de bancada, com métricas e estabilidade aprovadas. |
| 31/01/2027 | C — entrada no campo | Montagem final, RF, autonomia, invólucro e rotina aprovados para uso. |
| 14/02/2027 | D — campo | Duas rodadas em dias distintos, com >=4 h de coleta por unidade por rodada, nas cinco PCBs. |
| 28/02/2027 | E — aceite | PCB v1 funcional/documentada e relatório de piloto aprovado, sem bloqueador crítico. |

PDR = pacotes válidos únicos recebidos / tentativas reais de transmissão por unidade. Usar referência de TX independente dos dados que chegaram à API; separar perdas de rádio, fila e ingestão. Sem denominador confiável, a métrica é inconclusiva. Meta inicial: >=95% por unidade em bancada e nas janelas de cobertura declaradas para o campo; zero troca de identidade, corrupção ou duplicata lógica. Registrar latência P50/P95, fix GNSS, reinícios e autonomia; definir limites coerentes com a tarefa na S01 e congelá-los antes do Gate A. Não mudar o cenário depois para esconder perdas.

## Fabricação e caminho crítico

Base física (S08–S09) → esquema (S10) → layout (S11) → revisão/pedido (S12) → fabricação/transporte → montagem (S17) → integração/lote (S18–S19) → montagem final aprovada (S20) → campo (S21–S22) → aceite (S24).

**Meta de liberação/pedido: até 06/12/2026; recebimento planejado: antes de 04/01/2027.** A janela é uma premissa do calendário, não um prazo confirmado de fornecedor. Confirmar disponibilidade de componentes, custo, fabricação, montagem quando aplicável, frete e recebimento antes de contratar. Acompanhamento ativo entra no teto; S13–S14 usam a montagem de referência durante a espera. Nenhuma compra foi feita por este planejamento.

A reserva de 18 h cobre retrabalho ativo e indisponibilidade, não cria semanas extras para uma segunda fabricação. Não há uma segunda rodada completa de PCB garantida antes de fevereiro. Defeito que exija refabricação ou atraso de recebimento deve disparar replanejamento imediatamente. Não substituir silenciosamente a PCB do piloto pelo protótipo anterior.

## Controle de escopo e risco

- Conferir estimativas/experiência e disponibilidade na S01. S02 (GNSS/serial), S03 (segurança/autorização), S09 (mecânica/RF) e S17 (montagem) têm concentração de trabalho e merecem revisão antecipada.
- Reavaliar em **01/11 (Gate A), 15/11 (base física), 06/12 (pedido), 10/01 (montagem) e 24/01 (lote testado)**. Ausência de evidência impede avançar; datas não aprovam circuitos nem uso em animais.
- O software fica focado em um gateway, vínculos fixos, cadastro, identidade/tempo, ingestão e recuperação. Automação completa de CI, multi-gateway, sono avançado e produto completo não devem disputar horas com a v1; mudanças necessárias à segurança ou aos critérios do piloto continuam bloqueadoras.
- Ao fim de cada semana, registrar horas reais, evidência, pendência e reserva restante. Se a estimativa exceder a capacidade restante, explicitar o déficit e decidir capacidade, escopo técnico ou prazo com o responsável. Não remover critérios físicos ou de integridade para manter a data.
- O aceite da PCB em bancada na S19 não substitui o aceite de uso no animal na S20. Não avançar para campo com defeito elétrico, montagem inadequada ou energia insuficiente.

## Como acompanhar no GitHub

[Project dos dois repositórios](https://github.com/users/MateussGont/projects/1) · [Checklist semanal #32](https://github.com/MateussGont/cattle-tracker-lora/issues/32) · [PCB v1 #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) · [Piloto #22](https://github.com/MateussGont/cattle-tracker-lora/issues/22) · [Marco de fevereiro](https://github.com/MateussGont/cattle-tracker-lora/milestone/5).

Esta revisão é a referência vigente para datas e horas. Atualizações históricas do Project permanecem como registro, sem integrar o compromisso de 144 h. Pontos antigos não são horas. As fases M1–M4 continuam organizando a evolução posterior; [backlog](backlog.md) e [plano de evolução](plano-evolucao.md) preservam os demais objetivos.
