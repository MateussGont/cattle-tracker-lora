# Plano de evolução — Cattle Tracker
A prioridade é obter um brinco identificável e um fluxo de dados confiável, demonstrados em bancada, antes de ampliar o rebanho ou fechar a PCB. O trabalho será organizado por evidência de conclusão.

## Prazo da PCB v1 e do piloto
**PCB v1 do brinco fabricada, montada e testada + piloto validado até 28/02/2027, com 6 horas semanais para toda a equipe.** O [planejamento semanal](planejamento-semanal.md) distribui 144 h em 24 semanas: 126 h de entregas e 18 h de reserva. A previsão considera cinco brincos com a placa v1 própria, um gateway e uma propriedade, com duas rodadas de campo depois da bancada e da aprovação física. O [plano da PCB](plano-pcb-v1.md) define pacote de fabricação e evidências de aceite.

As fases abaixo descrevem a evolução completa. Somente os recortes identificados no calendário entram no compromisso de fevereiro; 10–25 unidades, RF discreto, miniaturização final, PCB do gateway e preparação integral de produção ficam para depois. Cumprir um recorte não encerra automaticamente sua issue mais ampla.

## Organização
- **cattle-tracker-lora:** brinco, gateway, protocolo do rádio, hardware, orçamento de enlace e validação física.
- **cattle-tracker-web:** backend, frontend, banco, MQTT de infraestrutura, operação do aplicativo e cadastro.
- **Project Cattle Tracker — Brinco LoRa MVP:** quadro central dos dois repositórios.
- Um responsável assume cada item quando ele entra em execução. Não atribuir automaticamente todo o backlog a uma pessoa.
- Limite de trabalho em andamento: uma entrega principal por vez, incluindo implementação e validação no teto de 6 h da equipe.
- Revisão semanal curta dentro das 6 h: resultado demonstrado, horas reais, bloqueios e próxima entrega. O calendário semanal substitui as datas históricas do roadmap como referência para fevereiro.

## Prioridades e fluxo
P0: impede acesso seguro ou uma validação básica confiável. P1: necessário antes de ampliar o piloto. P2: evolução posterior.
Backlog → Pronto → Em andamento → Em revisão/Validação → Concluído. Item bloqueado registra o motivo, a dependência e o responsável por desbloquear. A fase indica onde a entrega pertence, não se a implementação já começou.
Uma issue só está concluída quando seus critérios têm evidência vinculada: teste, log, documento, PR ou resultado físico. Construir firmware não comprova funcionamento das placas; criar cadastro não comprova rádio; receber MQTT não comprova persistência correta.

## Fase de organização — entregue em setembro de 2026
Separação de repositórios, contrato de integração documentado, manual do operador, revisão técnica do firmware e backlog rastreável publicados. O histórico anterior permanece no repositório original e o ponto de extração está identificado pela tag pre-web-split-4a85dff8.
Gate: fontes copiadas verificadas, fronteiras claras, site sem compilação implícita de firmware, instrução de importação/rollback e issues no quadro central.

## M1 — bancada confiável com um brinco
Ordem: HTML seguro do mapa e isolamento de sessão; pinagem GNSS; comandos seriais responsivos e identidade persistente; separação de tempos e ordenação; transação da ingestão; alertas e associações consistentes; teste ponta a ponta.
Metas iniciais de bancada (propostas de engenharia): comandos de status/configuração respondem em até 2 s no modo de serviço; zero mistura de identidades; zero duplicação lógica ao reenviar o mesmo evento; zero regressão da posição atual por mensagem antiga; 2 h contínuas com versão/configuração registradas.
GNSS sem fixação, GNSS desconectado, reboot de placa/API e falha de banco devem produzir estados distinguíveis.
Gate: uma unidade cadastrada seguindo o manual, validada após reinício e rastreada corretamente do rádio ao histórico. Erros de segurança P0 resolvidos antes de uso por contas distintas.

## M2 — 5, 10 e 25 brincos
Implementar recepção assíncrona, buffer com política explícita, heartbeat do gateway, identidade por inicialização e observações por receptor; publicar métricas de TX/RX/fila/descartes/duplicação.
Avaliar perfil SF7/BW125/CR4/5 com 60 s e variação temporal como ponto de partida experimental. Comparar com 30 s e o perfil de bancada. A escolha final depende de alcance, atraso tolerável e consumo.
Teste de 24 h por configuração candidata, ampliando uma escala por vez. Definir PDR medido sobre tentativas TX conhecidas, latência separada por trecho e motivo de descartes. Simulador mede backend; capacidade RF requer transmissores reais.
Metas provisórias: PDR ≥95% no cenário físico declarado, zero identidade trocada, ingestão sem duplicatas lógicas, recuperação automática documentada e nenhuma perda silenciosa de fila. Se a meta não for atingida, ajustar cadência/cobertura antes de multiplicar gateways ou aumentar SF.
Gate: resultados reproduzíveis de 25 nós; consumo, cobertura e recuperação de falhas medidos. Não tratar previsões de ALOHA como resultado de teste.

## M3 — viabilidade física e piloto controlado
Medir corrente em aquisição GNSS, TX e sono; selecionar medição de bateria e avaliar retenção da identidade/contador em reinícios. Testar GNSS e LoRa dentro do invólucro e próximo ao animal.
Validar massa, fixação, integridade mecânica, alimentação, antena e condições ambientais antes de fechar uma PCB. A PCB v1 própria do brinco passa a ser entrega obrigatória. A premissa é integrar os módulos já utilizados; a montagem modular serve aos ensaios antes da fabricação. O piloto de fevereiro usará as cinco PCBs v1, após nova aprovação física da montagem final.
Piloto progressivo: começar pequeno e ampliar para 10–25 unidades somente com evidência. Selecionar local, duração, responsáveis e critérios de interrupção com a equipe de manejo. Metas finais de autonomia, massa, cobertura e precisão precisam de dados e aprovação de produto.
Gate: relatório comparando resultados com metas, incidentes registrados e decisão de iterar hardware, ampliar piloto ou interromper.

## M4 — operação e produto
UX orientada às tarefas do operador, painel diagnóstico, paginação, trajetórias completas, releases independentes e compatibilidade explícita. Backup com restauração ensaiada, segredos por ambiente, métricas, políticas de retenção, TLS e procedimento de atualização/rollback.
Só ampliar número de instâncias do backend depois de definir propriedade do processamento MQTT/jobs e distribuição de eventos WebSocket.

## Ordem semanal vigente
S01 prepara ambiente, executor/revisor e materiais; S02–S07 corrigem o fluxo mínimo e validam uma unidade. S08–S09 qualificam energia, formato e RF; S10–S12 entregam esquema, layout e revisão/pedido da PCB. S13–S14 trabalham o gateway durante a fabricação. S17 monta a v1; S18–S20 integram, ensaiam cinco placas e aprovam a montagem final. S21–S22 executam o campo; S24 entrega o conjunto. S15, S16 e S23 são reservas.
O [calendário completo](planejamento-semanal.md) é a referência vigente. Reavaliar em 01/11, 15/11, 06/12, 10/01 e 24/01. Pedido até 06/12 e recebimento antes de 04/01 dependem do fornecedor; uma segunda fabricação completa não está garantida pela reserva.

## Decisões registradas
Frontend e backend permanecem juntos na aplicação web; gateway físico continua junto ao firmware. Não há mudança para LoRaWAN nesta etapa. O brinco permanece transmissor simples no MVP. As configurações RF propostas são experimentos, não parâmetros de produção já aprovados. Não introduzir ACK obrigatório ou recepção contínua no brinco sem demonstrar benefício e custo energético.
