package serverHTTP;

import java.io.EOFException;
import java.io.IOException;
import java.net.Socket;
import java.nio.channels.SocketChannel;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;

public class Room {
	
	private ArrayList<WebSocketResponse> clients;  
	private String userName; 
	private ArrayList<Messages> messages; 
	private String roomName;
	Messages message; 
	Database connection; 
	
	public  Room (String namePassed, WebSocketResponse responsePassed, Database connect) throws SQLException, EOFException, IOException {
		connection = connect; 
		clients = new ArrayList<WebSocketResponse> (); 
		messages = new ArrayList<Messages> (); 
		clients.add(responsePassed); 
//Testing whether or not the name passed is correct at this point.	System.out.println(namePassed + "for real???!!!");
		messages = connection.getMessages(namePassed);
	//moved this out to a method - could work here if the other one breaks
//		for (Messages message: messages) { 
//			System.out.println(message);
//			responsePassed.sendMessageFromRoom(message);
//		}
		
		
	}
	//for each user that enters a chat room, creates a list in order that the server can send out the messages to the correct client list
	public synchronized void addUser(WebSocketResponse responsePassed) {
		clients.add(responsePassed); 
	}
	//if I ever want to go to the trouble of removing a user from the room (i.e. someone who is blocked from a room etc.
	public synchronized void removeUser() {
		
	}
	//accesses the database and pulls in all of the old messages so that ever user is in sync
	public void getUpdatesForWhenIWasOut(WebSocketResponse responsePassed) throws IOException {
		for (Messages message: messages) {
			responsePassed.sendMessageFromRoom(message); 
		}
		
	}
	//this posts the messages to the room and the room sends the message back to the Websocket that sent it.
	public synchronized void postMessagetoRoom(Messages message) throws IOException, SQLException {
		//This tests to see if the message sent actually makes it to the postMessagetoRoom location
		System.out.println("You got to the post Message");
		messages.add(message);
		roomName = message.getRoom(); 
		//this is updating the database so that all users are able to access the same information when they login to a particular chatroom
		connection.postMessage(roomName, message.getUser(), message.getMessage()); 
		for (WebSocketResponse client: clients) {
			client.sendMessageFromRoom(message); 
			//testing to make sure that the message is in its correct format
			System.out.println(message);
		}	
	}
}
