//similar to importing or including a library
var http = require('http');
var url = require('url');
var fs = require('fs');
var uc = require('upper-case');

http.createServer(function(req, res){
  res.writeHead(200, {'Content-Type': 'text/html'});
  res.write(uc("Hello World!"));
  return res.end();
}).listen(8080);
