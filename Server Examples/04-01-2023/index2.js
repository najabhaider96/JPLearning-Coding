
const express = require('express');

const http = require('http');
const Server = require('socket.io');

// const io = require('socket.io')(server, {
//     cors: {
//         origin: '*'
//     }

// import http from 'http';
//     import { Server } from 'socket.io';

const PORT = process.env.PORT || 4001;

const app = express();

const httpServer = http.createServer(app);

const io = require('socket.io')(httpServer, {
    cors: {
        origin: '*'
    }
});

// const io = new Server(httpServer, { cors: { origin: '*' } });

io.on('connection', async (client) => {
    console.log('Client Connected');
    console.log(client.id);

    client.on(topic_ws_receive, async (data) => {
        console.log('[WS Received] Data:', data);
    })
});


httpServer.listen(PORT, async () => {
    console.log(`Example app listening on port ${PORT}`)
})