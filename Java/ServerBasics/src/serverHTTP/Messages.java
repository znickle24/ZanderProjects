package serverHTTP;

import java.io.EOFException;
import java.io.IOException;
import java.io.Serializable;
import java.security.Timestamp;

public class Messages implements Serializable {
	private String username; 
	private String message; 
	private String roomName;
	private Timestamp timestamp; 
	public Messages(String roomNamePassed, String usernamePassed, String messagePassed) throws EOFException, IOException {
		roomName= roomNamePassed; 
		username = usernamePassed; 
		message = messagePassed; 
		System.out.println("The message object has this roomname: "+ roomName);
		
		
		
	}
	public String getUser() {
		return username;
	}
	public String getMessage() {
		return message; 
	}
	public String getRoom() {
		return roomName; 
	}
	public String toJSON() {
		String jsonReply = new String();
		int spacePosition = message.indexOf(" ") ; 
		jsonReply = "{\"user\":\"" + username+"\", "+"\"message\":\""+message+"\"}"; //needs to be in JSON form in order to be read by the client front end we wrote;
		System.out.println("What's up with the space in this message" +message);
		return jsonReply; 
	}
	
}
