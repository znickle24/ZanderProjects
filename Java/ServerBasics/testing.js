let ws = "ws://"+location.host;
let mySocket=new WebSocket(ws);
//handshake happens
mySocket.onopen =  function () {
mySocket.send("User: Zander Room: fun times"); 

console.log("Websocket is Working");
mySocket.send("test"); 
console.log("test sent"); 
mySocket.send("What's up Doc"); 
console.log("test2 sent");
}
mySocket.onmessage = function (event) {
window.setTimeout(function() {
	mySocket.send("Hey BroHey BroHey BroHey BroHey BroHey BroHey BroHey BroHey BroHey BroHey BroHHey BroHey BroHey BroHey BroHey BroHey BroHey BroHey BroHe"); 
}, 2000 ); 
 console.log(event.data); 
}