//Messages and Users

window.onload=function() {
	let body = document.getElementById('body'); 
	let mySocket = new WebSocket("ws://"+location.host);
	let replaceHttp = function () {
		console.log(this); 
		body.innerHTML = this.responseText;
		newhtml();
	}

	let username;  
	let roomName; 
	//make sure there's an open connection with the WebSocket
	mySocket.onopen = function(event) {
		//error checking to make sure that the connection is valid
		console.log("websocket open");
		let request = function (e) {
			//don't default and produce a new website location
			e.preventDefault(); 
			mySocket.send("join " + document.getElementById('room_name').value.toLowerCase());
			username=document.getElementById('user_name').value; 
			roomName=document.getElementById('room_name').value;

			//replacing the body of the original HTML file with another 
			let xhr=new XMLHttpRequest(); 
			xhr.open('GET', 'WebSocketClient.html'); 
			xhr.addEventListener('load', replaceHttp); 
			xhr.send(); 

			return false; 

		} 
		//the next couple lines are reacting to the click of the "Join" button 
		let submission = document.getElementById('submit'); 
		submission.addEventListener('click', request);

		
		//doesn't do anything right now given the fact that the room will be empty.
	}
	 
	let newhtml = function () {
		
	//storing the username in a value so that we can display who wrote each message
		// let welcomeTag = document.createElement('p');
		// let welcomeText = "Welcome " + username + " to room " + roomName + "!" ; 
		// let welcomeMessage = document.createTextNode(welcomeText); 
		// welcomeTag.appendChild(welcomeMessage); 
		// body.appendChild(welcomeTag); 
		
		let message; 
		let messageSubmit = function () {
			message = document.getElementById('message').value;  
			let messageOutput = username+ ' ' + message;
			mySocket.send(messageOutput); 
			console.log(messageOutput); 

		}

		let submitTheMessage = document.getElementById('submitMessage'); 
		submitMessage.addEventListener('click', messageSubmit); 


	}
	mySocket.onmessage=function (event) {
		//did I reach the onmessage spot?
		console.log('hello');
		let messObject=JSON.parse(event.data); 
		console.log(messObject);
		let messageP=document.createElement('p'); 

		let mText=messObject.user+messObject.message;
		  //test to make sure there's data here
		console.log(messObject.user); 
		console.log(mText); 
		// let messageNode = document.createTextNode(mText); 
		messageP.innerHTML = mText; 
		let messageNode = document.getElementById('chatHistory');
		messageNode.appendChild(messageP); 

		//needs further testing, but am trying to get the div to scroll to the bottom of the texts each time
     	// let objDiv = document.getElementById("chatHistory");
     	// objDiv.scrollTop = objDiv.scrollHeight;
}

	}	


}