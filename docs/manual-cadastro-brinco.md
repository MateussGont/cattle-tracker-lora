# Manual de provisionamento do brinco

Este fluxo vale para unidades com a mesma imagem genérica de firmware. O aplicativo não cria o cadastro antes de identificar fisicamente o brinco e o operador não escolhe o ID LoRa.

## Pré-requisitos

- conta administradora e API/banco disponíveis;
- Chrome ou Edge em HTTPS ou localhost;
- brinco XIAO ESP32-S3 com cabo USB de dados e firmware compatível;
- nenhuma outra ferramenta usando a porta serial.

Atualizar firmware é uma operação de manutenção, separada do provisionamento normal. A imagem de fábrica já deve conter o protocolo descrito abaixo.

## Identidades

| Campo | Origem | Pode mudar? |
| --- | --- | --- |
| `hardwareUid` | MAC de fábrica do ESP32-S3 | Não |
| `radioDeviceId` | Reserva transacional do backend, 1–65535 | Não no fluxo normal |
| `deviceIdentifier` | Nome de inventário, como BRINCO-0001 | Sim, no cadastro |
| animal/gateway | Associação de negócio no backend | Sim, sem regravar o brinco |

O valor zero é reservado para unidade não provisionada.

## Passo a passo

1. Abra **Dispositivos → Novo dispositivo** e conecte apenas o brinco desejado.
2. Selecione a porta USB. O aplicativo envia `get_info` e mostra UID, firmware e estado antes de gravar qualquer registro.
3. Informe a identificação de inventário e, opcionalmente, modelo, gateway e animal.
4. Clique **Reservar ID e configurar**. O backend cria/retoma uma sessão idempotente e é a única autoridade que escolhe o `radioDeviceId`.
5. O aplicativo envia UID, ID e revisão ao firmware. O firmware grava uma configuração versionada em NVS, relê e só então confirma.
6. Reinicie o brinco e clique **Verificar após reinício e ativar**. O backend só ativa o dispositivo se UID, ID, revisão e versão forem exatamente os reservados.
7. Confirme a primeira telemetria no gateway e no aplicativo. Bateria “—” continua esperada enquanto não houver medição física.

Se a conexão cair, repita o fluxo com o mesmo hardware. O backend retoma a sessão válida e não reserva outro ID. Uma unidade já provisionada recusa mudança casual de identidade; recuperação/recondicionamento deverá usar um fluxo administrativo separado.

## Protocolo serial

Cada mensagem ocupa uma linha JSON terminada por LF e inclui um `requestId` correlacionado:

```json
{"cmd":"get_info","requestId":"550e8400-e29b-41d4-a716-446655440000"}
{"cmd":"provision","requestId":"550e8400-e29b-41d4-a716-446655440001","hardwareUid":"A1B2C3D4E5F6","radioDeviceId":101,"configRevision":1}
```

`GET_STATUS` é aceito apenas para diagnóstico legado. `SET_RADIO_ID` é recusado para impedir alteração fora da sessão. Eventos espontâneos não são considerados resposta pelo aplicativo.

## Evidência de bancada

Registrar por unidade: `deviceIdentifier | hardwareUid | radioDeviceId | configRevision | firmware | gateway | animal | leitura após reinício | primeira telemetria | responsável`.

A compilação e os testes automatizados não substituem a validação com uma placa real, um navegador compatível e o backend conectado.
