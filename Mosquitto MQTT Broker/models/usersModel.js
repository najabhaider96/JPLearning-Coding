/*
Welcome to JP Learning
*/
const mongoose = require('mongoose');

const Schema = mongoose.Schema;

const UsersSchema = new Schema({
    _id: {
        type: String,
        required: true
    },
    firstName: {
        type: String,
        required: true
    },
    lastName: {
        type: String,
        required: true
    },
    technology: {
        type: String,
        required: false
    },
    age: {
        type: Number,
        required: false
    },
}, {
        _id: false,
        id: false,
        versionKey: false,
        strict: false
    }
);

module.exports = mongoose.model('Users', UsersSchema);