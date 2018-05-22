const mqtt = require('mqtt')
const client = mqtt.connect('mqtt://192.168.2.239', {  username: 'dev', password: 'dev101' })
client.on('connect', ()=> {
  client.subscribe('iot')
})

client.on('message', (topic, message) => {
  console.log(message.toString())
})

