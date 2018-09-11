window.onload = function () {
	let answerp; 
	let callback = function () {
		console.log(this); 
		if (answerp != null) {
			document.body.removeChild(answerp); 
		}
		answerp = document.createElement('p'); 
		let answertxt = document.createTextNode(this.responseText); 
		answerp.appendChild(answertxt); 
		document.body.appendChild(answerp); 
	}
	let callbackError = function () {
		console.log(this);
	}
	let timeoutError = function () {
		console.log(this); 
		let errortxt = document.createTextNode('Timeout Error'); 
		let errorp = document.createElement('p'); 
		errorp.appendChild(errortxt); 
		document.body.appendChild(errorp); 
	}


//letting the user enter the number in the browser
	let enterNumber = document.createElement('p'); 
	let num1T = document.createTextNode('Enter First Number'); 
	let inputNum1 = document.createElement('input'); 
	inputNum1.setAttribute('type', 'text'); 
//getting second value from the user
	let enterNumber2 = document.createElement('p'); 
	let num2T = document.createTextNode('Enter Second Number'); 
	let inputNum2 = document.createElement('input'); 
	inputNum2.setAttribute('type', 'text'); 
//creating button in order to know when to request something from the server
	let submit = document.createElement('button');
	let submitTxt = document.createTextNode('Calculate'); 
	submit.appendChild(submitTxt);  
	let linebreak = document.createElement('p'); 
	 
//this function creates a data field that matches the syntax needed for the server to understand the request
//creates a new http request, sends the data and listens for a response from the server
//either shows the 404 or shows the value of the two numbers added
	 let request = function () { 
		let data = 'x='+inputNum1.value+'&y='+inputNum2.value; 
		//test to make sure data is stored
		console.log(data);

		let xhr = new XMLHttpRequest(); 
		xhr.open('GET', '/calculate?'+data); 
		xhr.addEventListener('load', callback); 
		xhr.timeout = 2000;
		xhr.addEventListener('timeout', timeoutError);
		xhr.addEventListener('error', callbackError); 
		xhr.addEventListener('send', request); 
		xhr.send(); 
		
	}

	submit.addEventListener('click', request); 

//after everything is created, loading it to the screen for the user to interact with
	enterNumber.appendChild(num1T); 
	document.body.appendChild(enterNumber); 
	document.body.appendChild(inputNum1); 
	enterNumber2.appendChild(num2T); 
	document.body.appendChild(enterNumber2); 
	document.body.appendChild(inputNum2); 
	document.body.appendChild(linebreak); 
	document.body.appendChild(submit); 

	let answer = document.createElement('output')
	




}