/*
Welcome to JP Learning
*/
const express = require('express');
const mqtt = require('mqtt');
const shortId = require('shortid');
const mongoose = require('mongoose');

const app = express();
const port = 8080;
app.use(express.json());

const server = require('http').createServer(app);
const io = require('socket.io')(server, {
    cors: {
        origin: '*'
    }
});

// Import Models
const Users = require('./models/usersModel');
const Events = require('./models/eventsModel');

// HTTP POST Method
app.post('/users', async (req, res) => {
    console.log('POST Request');

    // res.send('Got a POST request')

    let user = req.body;
    console.log('body:', user)

    user = new Users(user);
    console.log('user1:', user)

    user = await user.save();
    console.log('user:', user)

    return res.status(201).json(user);
})

// HTTP GET Method
app.get('/users', async (req, res) => {
    console.log('GET Request');

    // res.send('Assalam-o-Alaikum Hello World! Welcome to JP Learning :)')

    let users = await Users.find({});

    console.log('users:', users)

    return res.status(200).json(users);
})

// HTTP GET by ID Method
app.get('/users/:id', async (req, res) => {
    console.log('GET Request');

    // res.send('Assalam-o-Alaikum Hello World! Welcome to JP Learning :)')

    console.log('id:', req.params.id)
    // let user = await Users.findById(req.params.id);
    let user = await Users.findOne({ _id: req.params.id });

    console.log('user:', user)

    return res.status(200).json(user);
})

// HTTP PUT Method
app.put('/users/:id', async (req, res) => {
    console.log('PUT Request');

    // res.send('Got a PUT request at / device')

    console.log('id:', req.params.id)
    console.log('body:', req.body)

    // let user = await Users.findByIdAndUpdate(req.params.id, req.body);
    let user = await Users.findOneAndUpdate({ _id: req.params.id }, req.body);

    user = await Users.findById(req.params.id);

    console.log('user:', user);

    return res.status(200).json(user);
})

// HTTP DELETE Method
app.delete('/users/:id', async (req, res) => {
    console.log('DELETE Request');

    // res.send('Got a DELETE request at /device')

    console.log('id:', req.params.id)

    // let user = await Users.findByIdAndDelete(req.params.id);
    // let user = await Users.findByIdAndRemove(req.params.id);

    // let user = await Users.findOneAndDelete({ _id: req.params.id });
    let user = await Users.findOneAndRemove({ _id: req.params.id });

    console.log('user:', user);

    return res.status(200).json(user);
})

// HTTP GET by DeviceId and Type Method
app.get('/events/deviceId/:deviceId/type/:type', async (req, res) => {
    console.log('GET Request for Events');

    // res.send('Assalam-o-Alaikum Hello World! Welcome to JP Learning :)')

    console.log('deviceId:', req.params.deviceId)
    console.log('type:', req.params.type)
    let events = await Events.find({ device_id: req.params.deviceId, type: req.params.type });

    console.log('events:', events)

    return res.status(200).json(events);
})

// WebSocket(Socket.IO)

io.on('connection', client => {
    console.log('Client Connected');
    console.log(client.id);

    client.on('JPLearning_SensorData', async (data) => {
        console.log('[WS Received] Data:', data);
    })
});


setInterval(() => {
    sendData('time', new Date().toTimeString());
}, 1000);

sendData = async (topic, msg) => {
    io.emit(topic, msg);
}

// MongoDB Connection Success
mongoose.connection.on('connected', async () => {
    console.log('MongoDb connected');
});

// MongoDB Connection Fail
mongoose.connection.on('error', async (err) => {
    console.log('Error connecting MongoDb', err);
});

server.listen(port, async () => {
    // Connect MongoDb
    await mongoose.connect('mongodb+srv://jplearning:KC8CdRpHe3QwPIm5@cluster0.7xqgz4q.mongodb.net/Practice?retryWrites=true&w=majority');

    console.log(`Example app listening on port ${port}`)
})

// MQTT(HiveMQ)

const client = mqtt.connect('mqtt://broker.hivemq.com:1883');
const topic = "JPLearning_SensorData";

client.on('connect', async () => {
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