const mqtt = require('mqtt')
const client = mqtt.connect('mqtt://192.168.2.239')
let count = 0;
client.on('connect', ()=> {
  setInterval(()=> {
    count++;
    client.publish('tvone', 'Hello from Server#' + count)
  }, 6000)
})
