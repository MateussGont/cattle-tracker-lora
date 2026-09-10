# PCB v1 do brinco — escopo, fabricação e aceite

## Entrega obrigatória de fevereiro

**Até 28/02/2027: PCB v1 do brinco fabricada, montada e testada, junto com o piloto validado.** O [calendário de 6 h semanais da equipe](planejamento-semanal.md) prevê cinco unidades funcionais da v1: montagem até 10/01, bancada do lote até 24/01, aprovação física até 31/01 e duas rodadas de campo até 14/02. São metas de planejamento, ainda sem fabricação contratada ou hardware validado.

A [issue #20](https://github.com/MateussGont/cattle-tracker-lora/issues/20) acompanha a placa; a [#22](https://github.com/MateussGont/cattle-tracker-lora/issues/22) acompanha o campo. Os arquivos CAD da PCB ainda não foram encontrados no repositório inspecionado. A disponibilidade de projeto externo, executor, revisor, ferramentas e orçamento deve ser confirmada na primeira semana.

## Arquitetura prevista para a v1

Premissa para caber no prazo: **PCB própria de integração dos módulos já utilizados** para processamento, LoRa e GNSS. A seleção exata, dimensões e conexões serão congeladas após os ensaios; o planejamento não aprova um circuito elétrico. Reutilizar os módulos reduz o volume de projeto novo, mas ainda exige conferir alimentação, pinagem, antenas, montagem e testes na placa real.

O escopo inclui alimentação, proteção e carga compatíveis com a bateria escolhida, medição real de tensão, acesso USB/programação, conectores, desacoplamento, pontos de teste e identificação de revisão/serial. Conferir as funções já existentes nos módulos para evitar duplicação ou conflito de circuitos. Não fixar pinos, tensões, valores ou footprints sem os componentes reais e seus documentos.

RF com chips discretos, antena impressa nova, PCB do gateway, dimensões finais de produto e uma segunda fabricação completa ficam fora desta estimativa. Se a massa ou o volume dos módulos inviabilizarem o brinco, isso reprova a premissa na avaliação física; não forçar o formato para preservar o calendário.

## Condições para iniciar o layout e liberar fabricação

- [ ] Unidade de referência com cadastro e telemetria corretos após reinício (Gate A, até 01/11).
- [ ] Alimentação, picos de corrente, bateria e circuito de medição ensaiados (#6 e #7, S08).
- [ ] Envelope, massa, conectores, instalação/remoção e acesso para carga/programação definidos com a equipe de manejo (#18/#21).
- [ ] Antena e recepção GNSS verificadas no invólucro representativo, com requisitos aplicáveis e cenário documentados (#19).
- [ ] Tamanho do pacote/cadência e capacidade para o recorte de cinco nós avaliados (#4); o teste do lote físico será repetido após a fabricação.
- [ ] Executor/revisor disponíveis, biblioteca/footprints conferidos, componentes adquiríveis e prazo/orçamento compatíveis.

Essas evidências formam o Gate H de **15/11**. A conclusão integral de issues de 10–25 nós, autonomia comercial ou certificação final não é pré-requisito da placa experimental; o recorte necessário ao piloto precisa de evidência própria. A checagem da placa final para uso no animal continua obrigatória depois da montagem.

## Pacote de projeto e fabricação

Versionar os arquivos editáveis do CAD escolhido, bibliotecas específicas e versão da ferramenta, esquema em PDF, desenho mecânico com dimensões, lista de materiais com referências/códigos/quantidades, arquivos de fabricação e roteiro de montagem/teste. O pacote deve permitir reproduzir exatamente a revisão encomendada.

Executar verificação elétrica do esquema (ERC), verificação do layout (DRC), correspondência entre esquema e placa e inspeção independente dos arquivos exportados. No KiCad, ERC verifica conexões do esquema; DRC e exportação de Gerbers/furação fazem parte do fluxo documentado. Essas verificações não substituem revisão de engenharia nem ensaios físicos. [ERC — KiCad](https://www.kicad.org/discover/schematic-capture/), [DRC e fabricação — KiCad](https://docs.kicad.org/9.0/en/pcbnew/pcbnew.html).

- [ ] Esquema e lista de materiais revisados, inclusive polaridade e funções de carga/proteção.
- [ ] Footprints e pinagem conferidos contra a peça física/documentação; impressão em escala real quando útil.
- [ ] Regras do fabricante, retorno de corrente, áreas livres de antena, fixação e acesso a teste verificados.
- [ ] ERC/DRC sem erros críticos; exceções remanescentes justificadas por escrito e aceitas pelo revisor.
- [ ] Gerbers/furação inspecionados em visualizador independente; revisão do pacote igual à revisão do CAD.
- [ ] BOM e posição/rotação dos componentes quando houver montagem contratada; estratégia de montagem dos módulos definida.
- [ ] Relatório de revisão independente e checklist de teste assinados pelo responsável técnico do projeto.

## Janela de fabricação

S10 (16–22/11): esquema/BOM, **6 h**. S11 (23–29/11): layout e revisão inicial, **6 h**. S12 (30/11–06/12): revisão independente, ajustes, fabricação e acompanhamento do pedido, **6 h**. S17 (04–10/01): montagem e testes elétricos iniciais, **6 h**. Total específico: **24 h**, além das semanas de base física e dos ensaios compartilhados no calendário. Trabalho de revisor, aprendizado e acompanhamento também entra nas horas da equipe; não pressupor esforço externo gratuito.

**Pedido aceito até 06/12/2026 e recebimento antes de 04/01/2027 são premissas que dependem de cotação e confirmação.** Verificar fabricação, montagem, frete, componentes e recebimento antes de contratar. Planejar quantidade que resulte em cinco placas funcionais e avaliar sobressalentes conforme o orçamento. O responsável aprova o gasto; este planejamento não fez encomendas.

Enquanto o fabricante trabalha, S13–S14 tratam gateway/rede com a montagem de referência. Natal/Ano-Novo permanecem reserva de disponibilidade. A reserva final de fevereiro pode cobrir correção e repetição de ensaio; não garante tempo para uma nova fabricação. Se a revisão ou o fabricante não cumprir a janela, registrar nova previsão imediatamente.

## Testes e aceite da PCB

1. Inspecionar montagem, orientação, polaridade, continuidade e possíveis curtos antes de energizar. Começar por uma placa e alimentação controlada; prosseguir com o lote após aprovar o comportamento inicial.
2. Conferir tensões, consumo e comportamento com bateria/carga, programação/USB, GNSS, TX LoRa e medição real da bateria. Comparar com a montagem de referência. Documentar falhas e retrabalho por serial.
3. Montar e identificar cinco unidades funcionais. Vincular revisão do CAD/BOM, firmware, configuração, ID no software e unidade física; incluir essa conferência no manual de cadastro.
4. Repetir consumo e autonomia para a janela de campo na PCB final. Testar reinício, perda de rede, reconexão e leitura inválida. Não declarar autonomia comercial com base neste experimento.
5. Executar 24 h de bancada com cinco placas, TX conhecido e PDR >=95% por unidade no cenário declarado, sem mistura de identidade, duplicata lógica ou corrupção. Registrar latência, perdas de fila e recuperação separadamente.

**Aceite de bancada da #20: até 24/01/2027**, com lote funcional, fontes de fabricação e evidências. Uso em campo requer uma segunda aprovação da montagem final no invólucro: antena, energia, massa/fixação, instalação/remoção e inspeções da matriz definida para o piloto. **Aceite de campo: #22**, após duas rodadas nas cinco PCBs. Nenhuma etapa pode ser substituída por renderização ou arquivo de fabricação.

## Evolução após o piloto

Registrar o que a v1 demonstrou, falhas, alterações da v1.1/v2, redução de massa/volume, consumo e custo. Expansão para 10–25 unidades e projeto de produto completo dependem desses resultados. Se a placa não puder ser usada no animal, a entrega conjunta PCB + piloto permanece incompleta, mesmo com bancada aprovada.
