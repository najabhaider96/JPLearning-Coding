/*
Welcome to JP Learning
*/
const moment = require('moment');


console.log('Hello World');

let dateTime = moment().utc().add(5, 'hours');
console.log('Date and Time:',dateTime)
