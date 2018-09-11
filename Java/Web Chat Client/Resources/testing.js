window.onload=function() {
	let body = document.getElementById('body'); 
	let mySocket = new WebSocket("ws://"+location.host);
	mySocket.onerror = function (event) {
		console.log("There's an error"); 
	}
	//make sure there's an open connection with the WebSocket
	mySocket.onopen = function(event) {
		console.log("The socket is open"); 
	}
	 
	let newhtml = function () {


	}



}