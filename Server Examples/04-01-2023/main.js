var mqtt = require('mqtt')

var options = {
    host: '5c84fd61f11945979bbd077cdd2df908.s1.eu.hivemq.cloud',
    port: 8883,
    protocol: 'mqtts',
    username: 'Device0001',
    password: 'Device0001'
}

// initialize the MQTT client
var client = mqtt.connect(options);

// setup the callbacks
client.on('connect', function () {
    console.log('Connected');
});

client.on('error', function (error) {
    console.log(error);
});

client.on('message', function (topic, message) {
    // called each time a message is received
    console.log('Received message:', topic, message.toString());
});

// subscribe to topic 'my/test/topic'
client.subscribe('my/test/topic');

// publish message 'Hello' to topic 'my/test/topic'
client.publish('my/test/topic', 'Hello');