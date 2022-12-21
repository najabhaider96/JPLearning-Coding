/*
Welcome to JP Learning
*/
const mqtt = require('mqtt');
const mongoose = require('mongoose');
const shortId = require('shortid');

const Events = require('./eventsModel');

const client = mqtt.connect('mqtt://broker.hivemq.com:1883');

const topic = "JPLearning_SensorData";

// MongoDB Connection Success
mongoose.connection.on('connected', async () => {
    console.log('MongoDb connected');
});

// MongoDB Connection Fail
mongoose.connection.on('error', async (err) => {
    console.log('Error connecting MongoDb', err);
});

client.on('connect', async () => {
    // Connect MongoDb.
    await mongoose.connect('mongodb+srv://jplearning:KC8CdRpHe3QwPIm5@cluster0.7xqgz4q.mongodb.net/Practice?retryWrites=true&w=majority');

    console.log('MQTT Connected');

    client.subscribe(topic);
})

client.on('message', async (topic, message) => {
    console.log('MQTT Received Topic:', topic.toString(), ', Message:', message.toString());

    let data = message.toString();
    data = JSON.parse(data);
    data._id = shortId.generate();

    await saveData(data);
})

saveData = async (data) => {
    data = new Events(data);
    data = await data.save();
    console.log('Saved data:', data);

    await sendData('JPLearning_SensorData', data);
}

// Websocket

// const io = require('socket.io')({
//     cors: {
//         origin: '*'
//     }
// });

// io.on('connection', client => {
//     console.log('Client Connected');
//     console.log(client.id);

// });

// io.listen(8080);




// const server = require('http').createServer();
// const io = require('socket.io')(server, {
//     cors: {
//         origin: '*'
//     }
// });

// io.on('connection', client => {
//     console.log('Client Connected');
//     console.log(client.id);
// });

// server.listen(8080, async () => {
//     console.log('Server running on port 8080');
// });



const app = require('express')();
const server = require('http').createServer(app);
const io = require('socket.io')(server, {
    cors: {
        origin: '*'
    }
});

io.on('connection', client => {
    console.log('Client Connected');
    console.log(client.id);

    client.on('JPLearning_SensorData', async (data) => {
        console.log('[WS Received] Data:', data);
    })
});

server.listen(8080, async () => {
    console.log('Server running on port 8080');

});



setInterval(() => {
    sendData('time', new Date().toTimeString());
}, 1000);

sendData = async (topic, msg) => {
    io.emit(topic, msg);
}