package serverHTTP;

public class BadRequestException extends Exception {
	
	BadRequestException () {
		super ("404 / Bad HTTP Request"); 
	}
	
}

