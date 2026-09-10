// Analytical planning model, not a field capacity or regulatory certification.
// LoRa explicit header, payload CRC on, BW 125 kHz, CR 4/5, preamble 8, payload 26 B.
function airtime(sf, bytes=26, bandwidth=125000, cr=1, preamble=8) {
  const ts = 2 ** sf / bandwidth;
  const de = ts >= 0.016 ? 1 : 0;
  const payloadSymbols = 8 + Math.max(Math.ceil((8 * bytes - 4 * sf + 28 + 16) / (4 * (sf - 2 * de))) * (cr + 4), 0);
  return (preamble + 4.25 + payloadSymbols) * ts;
}
const rows = [];
for (const sf of [7,8,9,10,11,12]) for (const nodes of [5,10,25,50,100]) for (const interval of [10,12.5,30,60,300]) {
  const seconds=airtime(sf);
  const offeredLoad=nodes * seconds / interval;
  rows.push({sf,nodes,intervalSeconds:interval,airtimeMs:Number((seconds*1000).toFixed(3)),offeredLoadPercent:Number((offeredLoad*100).toFixed(3)),idealAlohaSuccessPercent:Number((Math.exp(-2*offeredLoad)*100).toFixed(2))});
}
console.log(JSON.stringify({assumptions:'Single channel/SF; independent random attempts; explicit header; PHY CRC on; 26-byte payload; no capture, interference, retransmissions or correlated timing. ALOHA estimate is illustrative, not measured PDR.',rows},null,2));
