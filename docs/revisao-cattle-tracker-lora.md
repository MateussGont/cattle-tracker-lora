Revisão de código e arquitetura — Cattle Tracker LoRa

Revisão da branch main no commit [4a85dff8](https://github.com/MateussGont/cattle-tracker-lora/commit/4a85dff8f089703848c17a84e3efa5dc6f7a9943), obtido do repositório informado. A análise cobre firmware do colar/receptor, ingestão MQTT/HTTP, persistência, autenticação e escopo por propriedade, alertas, frontend, provisionamento e configuração de execução.

A estrutura é adequada para evoluir o MVP: firmware separado, backend organizado em rotas/serviços/repositórios, PostgreSQL/PostGIS e frontend independente. Eu manteria essa base. Antes de confiar no sistema em campo, priorizaria segurança, integridade temporal da telemetria, comportamento durante desconexões e validação com hardware. A presença das funcionalidades e os testes de funções auxiliares ainda não demonstram funcionamento de ponta a ponta.

**Como interpretar a revisão**

P1 significa corrigir antes de uso compartilhado ou de um piloto que dependa da funcionalidade afetada. P2 significa corrigir no ciclo seguinte ou antes de aumentar a escala. “Reproduzido isoladamente” significa execução das funções TypeScript originais com dependências externas substituídas; não equivale a um teste integrado com banco, navegador ou placas. As recomendações de arquitetura são propostas, não alterações já aplicadas.

**Achados prioritários**

1. **[P1] Conteúdo cadastrado pode executar código no navegador pelo popup do mapa.**

O nome e o brinco são interpolados em HTML e enviados a `Popup.setHTML`. A validação no backend permite texto com marcação. Um usuário com permissão para cadastrar/editar animais pode armazenar conteúdo que será interpretado no navegador de quem abrir o popup. Como o token fica no localStorage, o impacto potencial inclui comprometimento da sessão.

Referências: [frontend/src/pages/MapPage.tsx:14](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/pages/MapPage.tsx#L14), [frontend/src/components/MapView.tsx:109](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/components/MapView.tsx#L109), [backend/src/schemas/animal.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/schemas/animal.ts), [frontend/src/contexts/AuthContext.tsx:45](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/contexts/AuthContext.tsx#L45).

Verificação: uma marcação HTML inofensiva inserida no nome permaneceu intacta na saída da função original. A execução de script no navegador não foi realizada. A [documentação oficial do MapLibre](https://maplibre.org/maplibre-gl-js/docs/API/classes/Popup/#sethtml) confirma que setHTML não sanitiza conteúdo.

Correção: construir elementos com textContent/setDOMContent ou renderização que escape os campos, mantendo apenas o layout sob controle da aplicação. Testar nomes/brincos contendo marcação e aspas.

2. **[P1] Os pinos GNSS do firmware contradizem a ligação documentada.**

O firmware define RX=43 e TX=44, mas o README orienta ligar o TX do GNSS ao D7/GPIO44. Seguindo o README, o programa escuta no pino diferente daquele que recebe os dados, impedindo a leitura GNSS. A UART do ESP32 pode ser remapeada; o defeito confirmado é a incompatibilidade entre firmware e ligação prescrita, não uma impossibilidade de usar esses GPIOs.

Referências: [firmware/collar/main.cpp:23](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/firmware/collar/main.cpp#L23) e [README.md](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/README.md). A [pinagem da Seeed](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#hardware-overview) confirma D6=GPIO43 e D7=GPIO44.

Correção: alinhar RX/TX ao circuito físico, atualizar comentários e documentação e verificar o recebimento de sentenças NMEA na placa.

3. **[P1] Trocar de usuário preserva dados do usuário anterior no cache.**

O QueryClient é criado uma vez, fora do AuthProvider. Logout remove token e usuário, mas não limpa nem separa as consultas por identidade. As chaves de mapa, dashboard, animais e propriedades são reutilizadas. Na mesma aba, sair da conta A e entrar na B pode exibir dados em cache de A, mesmo que as novas requisições sejam corretamente filtradas pelo backend. Consultas ainda consideradas recentes podem não buscar imediatamente.

Referências: [frontend/src/main.tsx](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/main.tsx), [frontend/src/contexts/AuthContext.tsx:50](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/contexts/AuthContext.tsx#L50), [frontend/src/hooks/useDashboard.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/hooks/useDashboard.ts), [frontend/src/hooks/useAnimals.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/hooks/useAnimals.ts).

Evidência: inspeção estática; não foi executada troca de contas no navegador.

Correção: cancelar consultas pendentes e descartar o cache ao trocar a sessão; considerar um QueryClient por sessão e chaves com identidade/escopo. Testar duas contas sem propriedades em comum, incluindo respostas atrasadas da primeira conta.

4. **[P1] O horário GNSS controla indevidamente a presença online.**

O serviço usa recordedAt, derivado do GNSS, como seenAt. Uma mensagem recebida agora com horário antigo faz o dispositivo aparecer offline; um horário futuro pode mantê-lo online artificialmente. No colar, gnssUnixTime verifica isValid, mas não a idade de data/hora; após interromper o GNSS, o último horário válido pode continuar sendo transmitido.

Referências: [backend/src/services/telemetryService.ts:49](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/telemetryService.ts#L49), [backend/src/repositories/devicesRepository.ts:91](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/devicesRepository.ts#L91), [firmware/collar/main.cpp:59](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/firmware/collar/main.cpp#L59).

Verificação: a função original de ingestão recebeu uma mensagem com horário GNSS de duas horas atrás; o heartbeat gerado levou a função de status a retornar offline.

Correção: separar receivedAt do servidor e sampledAt do GNSS. Usar receivedAt para comunicação e validar idade/faixa do horário GNSS. Manter a idade da posição separada da idade da conexão.

5. **[P1] Pacotes duplicados e atrasados alteram histórico e estado atual.**

Sequence é validado e transmitido, mas não utilizado nem armazenado pelo backend. O mesmo pacote recebido por dois gateways gera duas posições. Um pacote antigo processado depois de um recente substitui lastSeen e coordenadas no cache. O processamento MQTT inicia tarefas assíncronas sem serialização por dispositivo, permitindo também conclusões fora de ordem.

Referências: [backend/src/services/telemetryService.ts:39](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/telemetryService.ts#L39), [backend/src/integrations/mqtt/telemetrySubscriber.ts:19](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/integrations/mqtt/telemetrySubscriber.ts#L19), [backend/src/repositories/devicesRepository.ts:91](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/devicesRepository.ts#L91), [backend/src/db/schema.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/db/schema.ts).

Verificação: duas ingestões idênticas invocaram duas inserções de localização; a sequência recente→antiga fez o horário do heartbeat recuar. Os repositórios foram substituídos por registros em memória; não houve teste de concorrência no PostgreSQL.

Correção: introduzir identidade de evento e deduplicação atômica; atualizar o estado atual somente quando o evento for mais recente. A sequência zera ao reiniciar o colar, portanto deviceId+sequence isoladamente não basta: é necessário identificar a inicialização/sessão ou definir outra identidade resistente a reinícios. Preservar observações de gateways diferentes sem multiplicar o evento de localização.

6. **[P1] A ingestão pode ficar parcialmente persistida e não recupera falhas.**

Heartbeat, posição e alertas são gravados em operações separadas. Uma falha na inserção da localização pode deixar o mapa atualizado e o histórico incompleto. No caminho MQTT, o erro é apenas registrado, sem persistência de pendência ou tentativa posterior na aplicação.

Referências: [backend/src/services/telemetryService.ts:54](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/telemetryService.ts#L54), [backend/src/integrations/mqtt/telemetrySubscriber.ts:39](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/integrations/mqtt/telemetrySubscriber.ts#L39).

Correção: delimitar transação para o evento aceito e sua projeção; gravar trabalho/eventos pendentes de forma durável e só publicar atualizações externas após confirmação. Definir recuperação para mensagens cujo processamento falhou, combinada com deduplicação.

7. **[P1] O provisionamento serial falha conforme o momento e a divisão dos dados recebidos.**

readJsonLine examina somente a primeira linha de cada leitura antes de esperar a próxima. Se uma mensagem de depuração e a resposta JSON vierem juntas, a segunda linha pode ficar sem processamento. A função também aceita eventos espontâneos como awaiting_provisioning como se fossem a resposta. Além disso, o navegador espera 8 segundos, mas um colar já provisionado pode demorar mais de 12,5 segundos para voltar a atender comandos, devido à janela GNSS e ao delay de 10 segundos.

Referências: [frontend/src/lib/deviceProvisioning.ts:61](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/lib/deviceProvisioning.ts#L61), [frontend/src/lib/deviceProvisioning.ts:102](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/lib/deviceProvisioning.ts#L102), [firmware/collar/main.cpp:184](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/firmware/collar/main.cpp#L184), [firmware/collar/main.cpp:219](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/firmware/collar/main.cpp#L219).

Verificação: reproduzi a perda de um JSON válido no mesmo bloco de uma linha de log e o retorno de awaiting_provisioning no lugar de provisioned. A diferença de prazos foi confirmada por inspeção; não foi usada porta USB real.

Correção: processar todas as linhas disponíveis, manter buffer entre leituras, correlacionar comando/resposta, ignorar eventos não correspondentes e tornar o loop do colar responsivo. Bloquear operações simultâneas de gravação/configuração/status no assistente.

8. **[P1 para operação em campo] Desconexões descartam telemetria sem possibilidade de recuperação.**

Quando MQTT está desconectado, publishLocation retorna false e o receptor registra mqtt_publish_skipped; não há fila para reenviar. O publish usado é QoS 0. A assinatura QoS 1 no backend não transforma a publicação do gateway em entrega confirmada. Falhas de conexão também competem com a recepção LoRa no mesmo loop.

Referências: [firmware/receiver/mqtt_publisher.cpp:64](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/firmware/receiver/mqtt_publisher.cpp#L64), [firmware/receiver/main.cpp:59](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/firmware/receiver/main.cpp#L59), [backend/src/integrations/mqtt/client.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/integrations/mqtt/client.ts). O [código da PubSubClient 2.8](https://raw.githubusercontent.com/knolleary/pubsubclient/v2.8/src/PubSubClient.cpp) mostra a implementação do publish utilizado.

Correção: definir a perda tolerável e implementar armazenamento temporário com limite, política de descarte e reenvio. Avaliar publicação confirmada e sessão persistente. Telemetria reprocessada deve continuar idempotente. Desacoplar recepção de rádio e envio de rede.

9. **[P2] Reconhecer um alerta permite duplicá-lo e impede sua resolução automática.**

findOpenAlert e resolveOpenAlerts consideram apenas status=open. Depois de reconhecer um alerta, a próxima avaliação da mesma condição cria outro open; quando a condição acaba, o acknowledged permanece sem resolução automática. Há ainda uma corrida entre consultar e inserir, pois não existe unicidade para incidentes ativos.

Referências: [backend/src/repositories/alertsRepository.ts:44](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/alertsRepository.ts#L44), [backend/src/repositories/alertsRepository.ts:73](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/alertsRepository.ts#L73), [backend/src/services/alertService.ts:23](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/alertService.ts#L23).

Verificação: fluxo de reconhecimento e nova avaliação reproduzido com o predicado do repositório simulado; não executado no banco.

Correção: tratar open e acknowledged como o mesmo incidente ainda ativo, separar reconhecimento de encerramento e garantir unicidade atômica. Se a intenção for exigir encerramento manual após reconhecimento, documentar essa regra e impedir a duplicação.

10. **[P2] Ausência de medição resolve alerta de bateria; entidades nunca vistas não são avaliadas.**

batteryPercent=null faz o avaliador entrar no ramo de clearAlert, mesmo sem evidência de recuperação. O scheduler ignora lastSeen/lastGpsFixAt nulos, então uma coleira que nunca transmitiu ou nunca obteve fixação não gera esses alertas.

Referências: [backend/src/services/alertRuleEvaluator.ts:47](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/alertRuleEvaluator.ts#L47), [backend/src/services/alertRuleScheduler.ts:23](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/alertRuleScheduler.ts#L23).

Verificação: bateria nula provocou clearAlert; snapshots de dispositivo/gateway nunca vistos produziram zero avaliações.

Correção: diferenciar desconhecido, normal e violado. Para entidades ativadas sem primeira comunicação, avaliar o tempo desde ativação/provisionamento, com prazo de carência definido. A falta de medição não deve comprovar recuperação.

11. **[P2] A exclusividade da coleira por animal não está garantida no banco.**

A transação fecha associações anteriores, mas só existe índice único parcial por deviceId. Duas transações atribuindo dispositivos distintos ao mesmo animal sem associação prévia podem inserir dois vínculos ativos. As consultas limit(1) passam a escolher uma associação sem garantir qual, e joins de mapa/dashboard podem duplicar animais.

Referências: [backend/src/repositories/deviceAssignmentsRepository.ts:77](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/deviceAssignmentsRepository.ts#L77), [backend/src/db/schema.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/db/schema.ts), [backend/drizzle/0000_aromatic_darkstar.sql](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/drizzle/0000_aromatic_darkstar.sql), [backend/src/repositories/mapRepository.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/mapRepository.ts).

Evidência: inspeção do esquema, migrations e transação; corrida não executada no PostgreSQL.

Correção: índice único parcial também para animalId com unassigned_at IS NULL, migração que detecte dados inconsistentes e estratégia de bloqueio/tratamento de conflitos nas trocas.

12. **[P2] As cercas ativas são interpretadas como interseção obrigatória.**

A consulta seleciona toda cerca que não contém o animal, e qualquer resultado gera saída da propriedade. Com dois piquetes separados, estar dentro de um significa estar fora do outro: o alerta será permanente. Esse comportamento só é adequado se o requisito for que o animal esteja simultaneamente dentro de todas as áreas.

Referências: [backend/src/repositories/geofencesRepository.ts:50](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/geofencesRepository.ts#L50), [backend/src/services/telemetryService.ts:100](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/telemetryService.ts#L100).

Correção depende da regra de negócio: permitir a união das áreas ou associar cada animal ao piquete esperado. Definir também o tratamento da borda, tolerância ao erro GNSS e encerramento de incidentes quando uma cerca é removida. Este item exige confirmar a semântica pretendida antes da implementação.

13. **[P2] WebSocket não reconecta nem acompanha a expiração/revogação de acesso.**

O frontend cria a conexão uma vez e não trata fechamento para reconectar. Uma queda deixa o sistema dependente do polling de 60 segundos existente em mapa, dashboard e alertas. No servidor, token e propriedades são verificados apenas ao conectar; uma conexão aberta pode continuar recebendo eventos depois de expirar o token ou de remover o acesso à propriedade.

Referências: [frontend/src/hooks/useRealtimeUpdates.ts:24](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/hooks/useRealtimeUpdates.ts#L24), [frontend/src/hooks/useDashboard.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/hooks/useDashboard.ts), [frontend/src/hooks/useAlerts.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/hooks/useAlerts.ts), [backend/src/websocket/realtime.ts:37](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/websocket/realtime.ts#L37).

Correção: reconectar com espera progressiva e variação aleatória, refazer sincronização ao reconectar e encerrar/revalidar conexões por expiração e mudança de acesso. O middleware HTTP também usa o papel do JWT sem consultar estado atual da conta; definir explicitamente a política de revogação.

14. **[P2] Listas e histórico não representam todos os dados disponíveis.**

A API limita listas a 50 registros por padrão, enquanto telas e seletores não implementam paginação. O próximo ID de rádio é calculado no navegador a partir dessa lista parcial; após preencher a primeira página, pode sugerir IDs já usados. No histórico, os períodos de 7/30 dias solicitam apenas 2.000 pontos mais recentes e calculam a distância sobre esse subconjunto, sem indicar truncamento.

Referências: [backend/src/schemas/animal.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/schemas/animal.ts), [backend/src/schemas/device.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/schemas/device.ts), [frontend/src/components/DeviceProvisioningWizard.tsx:18](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/components/DeviceProvisioningWizard.tsx#L18), [frontend/src/pages/HistoryPage.tsx:40](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/pages/HistoryPage.tsx#L40), [backend/src/repositories/locationsRepository.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/locationsRepository.ts).

Impacto: animais/dispositivos fora da primeira página deixam de aparecer em seletores, e a distância do período fica incompleta. Com transmissões aproximadamente a cada 12,5 segundos, 2.000 pontos representam cerca de sete horas, antes de considerar perdas e ausência de fixação.

Correção: paginação com cursor/total ou busca em seletores; alocação de identidade no servidor; agregação da distância sobre o período completo e trajetória reduzida para visualização, sinalizando limites.

**Outros riscos concretos para o próximo ciclo**

- **Escopo do dispositivo é ambíguo.** Acesso e atribuição usam animal atual OU gateway. Um dispositivo cujo gateway pertence à propriedade A pode ser associado por um admin a um animal da B, sem mudar o gateway. Usuários de A continuam podendo consultar a coleira e dados do animal retornados com ela; gestores de A podem reassociá-la pelo ramo do gateway. O teste de autorização existente não cobre esse estado. Recomendo propriedade proprietária explícita no dispositivo, com recebimento por gateways modelado separadamente. Referências: [backend/src/routes/devices.ts:59](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/routes/devices.ts#L59), [backend/src/routes/animals.ts:134](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/routes/animals.ts#L134), [backend/src/repositories/devicesRepository.ts:31](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/devicesRepository.ts#L31).
- **Identidade do gateway não está ligada à credencial.** O HTTP usa chave global e o serviço confia no gatewayId do corpo; MQTT compartilha tópico e a configuração não inclui ACL por gateway. Não há validação de quais dispositivos aquele gateway pode reportar. Definir identidade autenticada, credenciais individuais e permissões por tópico/escopo antes de separar clientes/fazendas. Referências: [backend/src/middlewares/authenticateGateway.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/middlewares/authenticateGateway.ts), [backend/src/services/telemetryService.ts:40](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/telemetryService.ts#L40), [infra/mosquitto/mosquitto.conf](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/infra/mosquitto/mosquitto.conf).
- **“Gateway offline” depende de ouvir uma coleira conhecida.** lastSeen do gateway só é atualizado durante ingestão bem-sucedida de dispositivo provisionado. Um gateway ligado e conectado pode aparecer offline quando nenhuma coleira transmite; a conexão do receptor precisa de heartbeat independente. Referências: [backend/src/services/telemetryService.ts:54](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/telemetryService.ts#L54), [backend/src/repositories/gatewaysRepository.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/gatewaysRepository.ts).
- **O orçamento do pacote MQTT pode ser excedido.** PubSubClient 2.8 usa buffer padrão de 256 bytes para pacote, incluindo tópico/cabeçalho. Um payload ilustrativo de 236 bytes com gatewayId de 64 caracteres precisa de 267 bytes nesse caminho de publicação. O código só reserva 256 bytes para JSON e não aumenta o buffer MQTT nem verifica truncamento. Dimensionar os dois buffers e validar o tamanho serializado. Trata-se de cálculo de tamanho, sem execução Arduino. Referências: [firmware/receiver/mqtt_publisher.cpp:81](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/firmware/receiver/mqtt_publisher.cpp#L81), [buffer padrão](https://raw.githubusercontent.com/knolleary/pubsubclient/v2.8/src/PubSubClient.h), [verificação de tamanho](https://raw.githubusercontent.com/knolleary/pubsubclient/v2.8/src/PubSubClient.cpp).
- **Atualização parcial de regras pode produzir configuração incoerente.** As refinements validam apenas o patch; mudar somente metric preserva deviceId/gatewayId incompatível já salvo. Ler a regra, combinar o patch e validar o estado resultante antes de atualizar. Referências: [backend/src/schemas/alertRule.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/schemas/alertRule.ts), [backend/src/routes/properties.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/routes/properties.ts).
- **O cache da posição pertence ao dispositivo, não à associação.** Ao transferir uma coleira, o mapa do novo animal herda a posição anterior até chegar nova telemetria. Um pacote atrasado também é atribuído ao animal atual, mesmo se foi medido antes da troca. Usar a associação válida no instante do evento e exigir uma posição compatível com o vínculo atual. Referências: [backend/src/repositories/mapRepository.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/repositories/mapRepository.ts), [backend/src/services/telemetryService.ts:52](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/services/telemetryService.ts#L52).
- **Erros de domínio aparecem como erro interno.** Conflitos de ID/brinco e referências inválidas chegam ao tratamento genérico 500. Mapear conflitos esperados para 409 e dados inválidos para resposta apropriada, preservando logs técnicos no servidor. Referência: [backend/src/middlewares/errorHandler.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/middlewares/errorHandler.ts).
- **As instruções de execução estão defasadas.** O README diz que mesmo sem GNSS o colar transmite, mas o firmware atual não transmite até ser provisionado; a arquitetura também contém relatos históricos de “nesta sessão” que não descrevem verificações reproduzíveis. Atualizar com fluxo real, comandos de validação e resultados associados a versão.

**Arquitetura recomendada para a próxima etapa**

Manter o monorepositório e um backend modular é razoável para a fase atual. MQTT, PostGIS e React cumprem papéis definidos. O principal trabalho é explicitar os contratos entre as camadas e suas garantias.

O fluxo proposto é: colar cria evento identificado → gateway recebe e guarda pendências → transporte autenticado → backend valida e deduplica → transação registra evento/projeção → alertas e notificações são processados após confirmação → frontend atualiza apenas os dados afetados.

Na persistência, separar evento de telemetria, observação do gateway e posição válida. O evento deve permitir rastrear dispositivo, sessão de inicialização, sequência, horário medido, horário recebido e versão do protocolo; a observação preserva gateway, RSSI e SNR. Isso permite depurar perda/duplicação sem confundir um evento ouvido por dois receptores com duas medições.

No domínio, definir a propriedade proprietária de dispositivo e gateway, a regra de transferência de coleira, a semântica das cercas e a máquina de estados dos alertas. Essas decisões devem estar em serviços/regras compartilhadas por rotas, jobs e ingestão, apoiadas por restrições do banco.

Na interface, evitar refazer a consulta de todo o rebanho por pacote. Hoje cada location_update invalida mapa/dashboard; o mapa recria marcadores e reajusta o enquadramento quando os dados mudam. Preferir atualizações pontuais ou agrupadas, preservando o enquadramento escolhido pelo usuário. O rate limit global de 100 requisições/minuto precisa ser avaliado junto dessa carga e do fallback HTTP. Referências: [frontend/src/hooks/useRealtimeUpdates.ts:41](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/hooks/useRealtimeUpdates.ts#L41), [frontend/src/components/MapView.tsx](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/src/components/MapView.tsx), [backend/src/app.ts](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/backend/src/app.ts).

Não escalar o backend simplesmente aumentando o número de réplicas: cada instância assina o tópico inteiro e executa seu scheduler, e o registro de WebSockets é local ao processo. Primeiro definir quem processa cada evento/job e como distribuir notificações. Pode-se manter uma instância inicialmente e evoluir apenas após medir a carga.

Desacoplar também a compilação do frontend da compilação de firmware. Os hooks predev/prebuild exigem PlatformIO e compilam a placa até para trabalhar em uma tela. Publicar um firmware versionado, com manifesto, hash e compatibilidade de placa, consumido pelo assistente. Manter a geração como etapa explícita de release. Referências: [frontend/package.json](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/frontend/package.json), [scripts/sync-firmware.mjs](https://github.com/MateussGont/cattle-tracker-lora/blob/4a85dff8f089703848c17a84e3efa5dc6f7a9943/scripts/sync-firmware.mjs).

**Capacidade, operação e hardware**

A janela de leitura de 2,5 segundos somada ao delay de 10 segundos produz período superior a 12,5 segundos, não exatamente 10 segundos. A 1.000 coleiras e aproximadamente 12,5 segundos por envio, são cerca de 6,9 milhões de eventos por dia se todos forem recebidos; apenas eventos com fixação geram locations hoje. Esse cálculo ilustra volume de escrita, não capacidade demonstrada do enlace LoRa. Definir retenção, agregações de histórico e métricas de armazenamento antes de acumular meses de dados.

Validar separadamente capacidade do rádio, colisões entre coleiras, alcance com animal em movimento, consumo e autonomia. O código já documenta bateria sem medição, ausência de deep sleep, ACK e autenticação criptográfica do pacote. CRC detecta corrupção e não autentica a origem. Essas são limitações reais do protótipo; a revisão não mediu autonomia, alcance ou resistência física.

Infraestrutura atual descreve ambiente local: banco com credencial de exemplo e portas publicadas, MQTT sem TLS configurado, sem empacotamento de produção de API/frontend. Preparar um perfil de implantação com segredos, exposição de rede deliberada, backup/restauração, readiness do banco/broker e métricas de descarte, atraso e falhas de ingestão. /healthz atualmente só retorna ok e não mede dependências.

**Validação feita e limites**

- Clone limpo da branch main; não alterei código versionado nem publiquei issues, comentários ou commits.
- Compilei e executei o teste nativo C++ existente de protocolo com warnings habilitados. Resultado: protocol tests passed. Ele cobre ida/volta da codificação e rejeição após corrupção de um byte; não valida rádio físico.
- Executei sete cenários sobre funções TypeScript originais com dependências externas substituídas: horário antigo, duplicação, regressão temporal, bateria desconhecida, entidades nunca vistas e dois comportamentos do parser serial.
- Executei a geração original de HTML com marcação inofensiva e confirmei ausência de escape.
- Simulei o fluxo de reconhecimento/repetição de alerta com o predicado do repositório e calculei o orçamento de um pacote MQTT. Não confundir esses dois resultados com testes integrados.
- As dependências Node do projeto não estão instaladas no clone. Não executei as suítes Vitest, builds de backend/frontend, testes de integração com PostgreSQL/PostGIS/Mosquitto nem builds PlatformIO. Não instalei dependências.
- A suíte de autorização existente cobre alguns acessos cruzados e papéis, o que é útil, mas não cobre revogação em conexão aberta, cache entre usuários, dispositivo com gateway/animal de propriedades distintas, concorrência ou ingestão resiliente. Não há workflow de CI versionado no commit analisado.

**Ordem sugerida de execução**

1. Corrigir HTML do mapa, cache por sessão e divergência de pinagem; validar o provisionamento serial.
2. Definir identidade e tempos do evento; implementar deduplicação, ordenação e transação da ingestão.
3. Ajustar incidentes de alerta, desconhecido/recuperação e exclusividade de associação no banco.
4. Definir propriedade do dispositivo, regra de cercas e autorização do gateway; cobrir os cenários de acesso com testes.
5. Adicionar recuperação de rede, heartbeat do gateway, reconexão do frontend, paginação e histórico completo.
6. Separar releases de firmware, adicionar CI e executar um piloto instrumentado com falhas induzidas: GNSS indisponível, Wi-Fi fora, broker/API reiniciando, troca de coleira, pacotes duplicados e fora de ordem.

A revisão fornece uma base de correções verificáveis. Os critérios de aceite do piloto devem incluir perda tolerável de mensagens, atraso máximo de alerta, precisão da localização e autonomia desejada; esses requisitos não estão quantificados no material examinado.
