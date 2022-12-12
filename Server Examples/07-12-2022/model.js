/*
Welcome to JP Learning
*/
const mongoose = require('mongoose');

const Schema = mongoose.Schema;

const UsersSchema = new Schema({
    _id: String,
    firstName: String,
    lastName: String,
    technology: String,
    age: Number
}, {
        _id: false,
        id: false,
        versionKey: false,
        strict: false
    }

);


module.exports = mongoose.model('Users', UsersSchema);