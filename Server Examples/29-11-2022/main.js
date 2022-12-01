/*
Welcome to JP Learning
*/

console.log('Hello World :)')

var http = require("http");  

http.createServer(function (request, response) {  
   // Send the HTTP header   
   // HTTP Status: 200 : OK  
   // Content Type: text/plain  
   response.writeHead(200, {'Content-Type': 'text/plain'});  
   // Send the response body as "Hello World"  
   response.end('Assalam-o-Alaikum\nHello World\n\nWelcome to JP Learning\n');  
}).listen(3000);  
// Console will print the message  
console.log('Server running at http://127.0.0.1:3000/');  