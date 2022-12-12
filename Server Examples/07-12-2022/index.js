/*
Welcome to JP Learning
*/
const express = require('express')
const mongoose = require('mongoose')

const Users = require('./model')


const app = express()
const port = 8080


app.use(express.json());

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


// MongoDB Connection Success
mongoose.connection.on('connected', () => {
    console.log('MongoDb connected');
});

// MongoDB Connection Fail
mongoose.connection.on('error', (err) => {
    console.log('Error connecting MongoDb', err);
});


app.listen(port, async () => {
    await mongoose.connect('mongodb+srv://jplearning:KC8CdRpHe3QwPIm5@cluster0.7xqgz4q.mongodb.net/Practice?retryWrites=true&w=majority');

    console.log(`Example app listening on port ${port}`)
})
