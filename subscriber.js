const mqtt = require('mqtt')
const client = mqtt.connect('mqtt://192.168.2.239', {  username: 'dev', password: 'dev101' })
client.on('connect', ()=> {
  client.subscribe('iot')
})

client.on('message', (topic, message) => {
  const data = message.toString();
  const array = data.split(" ")
  const hex = array[5] + array[6] + array[3] + array[4]
  const tail = array[array.length - 1]
  console.log(parseInt(hex, 16)+ " " +tail)
})

