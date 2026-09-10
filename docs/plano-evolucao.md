# Plano de evolução — Cattle Tracker
A prioridade é obter um brinco identificável e um fluxo de dados confiável, demonstrados em bancada, antes de ampliar o rebanho ou fechar a PCB. O trabalho será organizado por evidência de conclusão.

## Organização
- **cattle-tracker-lora:** brinco, gateway, protocolo do rádio, hardware, orçamento de enlace e validação física.
- **cattle-tracker-web:** backend, frontend, banco, MQTT de infraestrutura, operação do aplicativo e cadastro.
- **Project Cattle Tracker — Brinco LoRa MVP:** quadro central dos dois repositórios.
- Um responsável assume cada item quando ele entra em execução. Não atribuir automaticamente todo o backlog a uma pessoa.
- Limite inicial de trabalho em andamento: duas implementações e uma validação de bancada para a equipe. Ajustar conforme a capacidade real.
- Reunião semanal curta: resultado demonstrado, bloqueios, próximos itens. Datas antigas do Project são estimativas históricas; replanejar somente depois de conhecer disponibilidade e materiais.

## Prioridades e fluxo
P0: impede acesso seguro ou uma validação básica confiável. P1: necessário antes de ampliar o piloto. P2: evolução posterior.
Backlog → Pronto → Em andamento → Em revisão/Validação → Concluído. Item bloqueado registra o motivo, a dependência e o responsável por desbloquear. A fase indica onde a entrega pertence, não se a implementação já começou.
Uma issue só está concluída quando seus critérios têm evidência vinculada: teste, log, documento, PR ou resultado físico. Construir firmware não comprova funcionamento das placas; criar cadastro não comprova rádio; receber MQTT não comprova persistência correta.

## Fase de organização — agora
Entregar separação de repositórios, contrato de integração documentado, manual do operador, revisão técnica do firmware e backlog rastreável. O histórico anterior permanece no repositório original e o ponto de extração será identificado por commit/tag.
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
Validar massa, fixação, integridade mecânica, alimentação, antena e condições ambientais antes de fechar uma PCB. Um protótipo modular pode participar do piloto se cumprir os requisitos físicos; PCB customizada não é um gate obrigatório por si só.
Piloto progressivo: começar pequeno e ampliar para 10–25 unidades somente com evidência. Selecionar local, duração, responsáveis e critérios de interrupção com a equipe de manejo. Metas finais de autonomia, massa, cobertura e precisão precisam de dados e aprovação de produto.
Gate: relatório comparando resultados com metas, incidentes registrados e decisão de iterar hardware, ampliar piloto ou interromper.

## M4 — operação e produto
UX orientada às tarefas do operador, painel diagnóstico, paginação, trajetórias completas, releases independentes e compatibilidade explícita. Backup com restauração ensaiada, segredos por ambiente, métricas, políticas de retenção, TLS e procedimento de atualização/rollback.
Só ampliar número de instâncias do backend depois de definir propriedade do processamento MQTT/jobs e distribuição de eventos WebSocket.

## Primeiros dois ciclos sugeridos
Ciclo 1: terminar os ajustes de organização e segurança web, corrigir GNSS/serial e preparar uma unidade/inventário. Produzir demonstração curta do cadastro e consulta após reinício.
Ciclo 2: evento idempotente e temporalmente correto, transação, alertas e teste de 2 h com falhas induzidas. Em paralelo ao desenvolvimento, levantar materiais e medir consumo do protótipo existente.
Os ciclos representam ordem de trabalho, não compromisso de prazo. Hardware, disponibilidade da equipe e revisão dos resultados determinam as datas.

## Decisões registradas
Frontend e backend permanecem juntos na aplicação web; gateway físico continua junto ao firmware. Não há mudança para LoRaWAN nesta etapa. O brinco permanece transmissor simples no MVP. As configurações RF propostas são experimentos, não parâmetros de produção já aprovados. Não introduzir ACK obrigatório ou recepção contínua no brinco sem demonstrar benefício e custo energético.
