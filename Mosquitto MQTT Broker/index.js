/*
Welcome to JP Learning
*/
const express = require('express');
const mqtt = require('mqtt');
const shortId = require('shortid');
const mongoose = require('mongoose');
const moment = require('moment');
const cors = require('cors');

const app = express();
const port = 8080;
app.use(express.json());

// Enable cors on all requests.
app.use(cors());
app.options('*', cors());

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

// HTTP GET last events of all devices
app.get('/events/:deviceId', async (req, res) => {
    console.log('GET Request for Events');

    let events = [];

    let eventConnection = await Events.findOne({ device_id: req.params.deviceId, type: 'Connection' }).sort({ created: -1 });
    if (eventConnection) {
        events.push(eventConnection);
    }

    let eventTemperature = await Events.findOne({ device_id: req.params.deviceId, type: 'Temperature' }).sort({ created: -1 });
    if (eventTemperature) {
        events.push(eventTemperature);
    }

    let eventHumidity = await Events.findOne({ device_id: req.params.deviceId, type: 'Humidity' }).sort({ created: -1 });
    if (eventHumidity) {
        events.push(eventHumidity);
    }

    let eventLight = await Events.findOne({ device_id: req.params.deviceId, type: 'Light' }).sort({ created: -1 });
    if (eventLight) {
        events.push(eventLight);
    }

    console.log('events:', events)

    return res.status(200).json(events);
})

// HTTP GET by DeviceId and Type Method
app.get('/events/deviceId/:deviceId/type/:type', async (req, res) => {
    console.log('GET Request for Events');

    console.log('deviceId:', req.params.deviceId)
    console.log('type:', req.params.type)
    let events = await Events.find({ device_id: req.params.deviceId, type: req.params.type });

    console.log('events:', events)

    return res.status(200).json(events);
})

// HTTP GET last event by DeviceId and Type Method
app.get('/events/last/deviceId/:deviceId/type/:type', async (req, res) => {
    console.log('GET Request for Last Events');

    console.log('deviceId:', req.params.deviceId)
    console.log('type:', req.params.type)
    // let eventTemperature = await Events.findOne({ device_id: req.params.deviceId, type: req.params.type }).sort({ created: -1 }).limit(1);
    let event = await Events.findOne({ device_id: req.params.deviceId, type: req.params.type }).sort({ created: -1 });

    console.log('event:', event)

    return res.status(200).json(event);
})

mongoose.set('strictQuery', false);
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
    await mongoose.connect('YOUR MONGODB LINK');

    console.log(`Example app listening on port ${port}`)
})
// WebSocket(Socket.IO)
const topic_ws_receive = 'JPLearning_CommandRequest';
const topic_ws_send = 'JPLearning_SensorData';

io.on('connection', async (client) => {
    console.log('WS Client Connected');
    console.log(client.id);

    client.on(topic_ws_receive, async (data) => {
        console.log('[WS Received] Data:', data);
        console.log('[WS Received] Data.device_id:', data.device_id);

        // Send command to device
        await sendToDevice(topic_mqtt_send, data);
    })
});

// setInterval(async () => {
//     await sendToApplicaiton('time', new Date().toTimeString());
// }, 1000);

sendToApplicaiton = async (topic, msg) => {
    console.log('\n[WS Sending] data to Applications message:', msg)
    await io.emit(topic, msg);
}

// MQTT(HiveMQ)
const topic_mqtt_receive_events = 'JPLearning_SensorData';
const topic_mqtt_receive_connections = '$SYS/broker/log/#';
const topic_mqtt_send = 'JPLearning_CommandRequest';

const ClientId = 'Application_' + Math.random().toString(16).substring(2, 8)
const options = {
    host: '192.168.2.150',
    port: 1883,
    protocol: 'mqtt',
    username: 'testing2',
    password: 'testing2',
    clientId: ClientId
}

const client = mqtt.connect(options);
// const client = mqtt.connect('mqtt://192.168.2.150:1883');
// const client = mqtt.connect('mqtt://localhost:1883');

client.on('connect', async () => {
    console.log('MQTT Connected');

    await client.subscribe(topic_mqtt_receive_events);

    await client.subscribe(topic_mqtt_receive_connections);
})
client.on('error', (error) => {
    console.error('MQTT client error:', error);
    // Handle the error here
});

// Regular expression patterns to extract Client ID and Username
const connectedRegex = /New client connected from .+ as (\S+) \(.+ u'([^']+)'\)/;
const disconnectedRegex = /Client (\S+) disconnected/;

client.on('message', async (topic, message) => {
    console.log('\n[MQTT Received] Topic:', topic, ', Message:', message.toString());

    if (topic.startsWith('$SYS/broker/log/')) {
        const logMessage = message.toString();
        let connectionEvent = false, clientId, status;

        // Check for connection event
        const connectedMatch = logMessage.match(connectedRegex);
        // Check for disconnection event
        const disconnectedMatch = logMessage.match(disconnectedRegex);

        if (connectedMatch) {
            clientId = connectedMatch[1];
            const username = connectedMatch[2];
            status = 1;
            connectionEvent = true;

            console.log('Connection Event:');
            console.log('Client ID:', clientId);
            console.log('Username:', username);
        } else if (disconnectedMatch) {
            clientId = disconnectedMatch[1];
            status = 0;
            connectionEvent = true;

            console.log('Disconnection Event:');
            console.log('Client ID:', clientId);
        }

        if (connectionEvent && clientId.startsWith('Deivce_')) {
            let data = {
                device_id: clientId.split('_')[1],
                type: 'Connection',
                value: status
            };
            data._id = shortId.generate();
            data.created = moment().utc().add(5, 'hours');

            // Save live data into database
            await saveData(data);
        }
    }

    if (topic == topic_mqtt_receive_events) {
        let data = message.toString();
        data = JSON.parse(data);
        data._id = shortId.generate();
        data.created = moment().utc().add(5, 'hours');

        // Save live data into database
        await saveData(data);
    }
})

saveData = async (data) => {
    data = new Events(data);
    data = await data.save();
    console.log('Saved data:', data);

    // Send live data to applications
    await sendToApplicaiton(topic_ws_send, data);
}

sendToDevice = async (topic, message) => {
    console.log('\n[MQTT Sending] data to Device message:', message)

    let data = JSON.stringify(message);
    // data = JSON.parse(data);
    await client.publish(topic, data);
}