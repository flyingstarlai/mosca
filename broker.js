const mosca = require('mosca')
const setting = { port: 1883 }
const server = mosca.Server(setting)
server.on('ready', () =>{
  console.log('ready...')
  server.authenticate =(client, username, password, callback) => {
    console.log('client try to connect with ' + username +'/'+password)
    if(username === 'dev' && password.toString() === 'dev101') {
      console.log('connected')
      callback(null, true)
    } else {
      console.log('invalid auth...')
      callback(null, false)
    }
  }
})

