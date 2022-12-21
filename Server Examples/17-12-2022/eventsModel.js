/*
Welcome to JP Learning
*/
const mongoose = require('mongoose');
const moment = require('moment');

const Schema = mongoose.Schema;

const EventsSchema = new Schema({
    _id: {
        type: String,
        required: true
    },
    device_id: {
        type: String,
        required: true
    },
    type: {
        type: String,
        required: true
    },
    value: {
        type: Number,
        required: true
    },
    created: {
        type: Date,
        default: moment().utc().add(5, 'hours')
    }
}, {
        _id: false,
        id: false,
        versionKey: false,
        strict: false
    }
);


module.exports = mongoose.model('Events', EventsSchema);