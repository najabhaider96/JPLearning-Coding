/*
Welcome to JP Learning
*/
const express = require('express')
const app = express()
const port = 8080

// HTTP GET Method
app.get('/test', (req, res) => {
    console.log('GET Request');
    res.send('Assalam-o-Alaikum Hello World! Welcome to JP Learning :)')
})

// HTTP POST Method
app.post('/', (req, res) => {
    console.log('POST Request');
    res.send('Got a POST request')
})

// HTTP PUT Method
app.put('/device', (req, res) => {
    console.log('PUT Request');
    res.send('Got a PUT request at / device')
})

// HTTP DELETE Method
app.delete('/device', (req, res) => {
    console.log('DELETE Request');
    res.send('Got a DELETE request at /device')
})



app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})
