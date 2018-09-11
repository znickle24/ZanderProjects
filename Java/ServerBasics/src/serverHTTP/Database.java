package serverHTTP;


import java.io.EOFException;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

public class Database {
	Connection connection; 
 
	public Database () throws Exception {
		  
		try {
		String url = new String(); 
		url = "jdbc:sqlite:server.db"; 
		connection = DriverManager.getConnection(url); 
		System.out.println("You're connected to the Database in SQL");
		} catch (SQLException e) {
			System.out.println(e.getMessage());
			System.out.println(e.getStackTrace());  
		} 
			
		
		
		Statement createTable = connection.createStatement();
		String sql = "CREATE TABLE IF NOT EXISTS MessageTable (roomName text, username text, message text)"; 
		createTable.execute(sql); 
	}	
	
//when a room is created, search for that room and get all of the messages for that initial user
	public void postMessage(String roomName, String username, String message) throws SQLException {
		String sql2 = "INSERT INTO MessageTable (roomName, username, message) VALUES(?, ?, ?)";
		System.out.println("In the post message function of the database, room name: " + roomName);
		PreparedStatement statement = connection.prepareStatement(sql2); 
		statement.setString(1, roomName);
		statement.setString(2, username);
		statement.setObject(3, message);
		statement.executeUpdate(); 
	}

	//for each message, make sure that they are posted to the database
	//need to 
	public ArrayList<Messages> getMessages(String roomName) throws SQLException, EOFException, IOException {
		System.out.println("you're now at the get messages");
		System.out.println(roomName);
		String sqlQuery = "SELECT roomName, username, message FROM MessageTable WHERE roomName =?";
		 
		PreparedStatement prepStatement = connection.prepareStatement(sqlQuery); 
		prepStatement.setString(1, roomName); 
		
		ResultSet resultingMessages = prepStatement.executeQuery();
		System.out.println("The resulting message in the getMessage function for database:" +resultingMessages);
		ArrayList<Messages> messageFromDatabase = new ArrayList<>(); 
		while (resultingMessages.next()) {
			Messages message = new Messages(resultingMessages.getString(1), resultingMessages.getString(2), resultingMessages.getString(3)); 
			System.out.println("The room showing in the get Messages method: " +resultingMessages.getString(1));
			messageFromDatabase.add(message); 
			System.out.println("The message that is added to the messageFrom Database:" + message);
			
		}
		return messageFromDatabase;
		
	}
	
}

