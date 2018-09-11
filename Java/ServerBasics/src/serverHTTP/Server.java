package serverHTTP;

import java.io.EOFException;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server {

	private int portNumber = 8080;
	private ServerSocketChannel serverSocket = null;
	private Socket clientSocket = null;
	private Selector sel;
	private HashMap<String, Room> roomObjects; 
	private Database connect; 

	public Server() throws Exception {
		 
		//create hash map
		roomObjects = new HashMap(); 
		serverSocket = ServerSocketChannel.open().bind(new InetSocketAddress(portNumber));
		serverSocket.configureBlocking(false);
		sel = Selector.open();
		serverSocket.register(sel, SelectionKey.OP_ACCEPT);
		connect = new Database();
	}

	public void serve() throws Exception {
		ExecutorService pool = Executors.newFixedThreadPool(10);
		while (true) {

			// creating a socket for the client, listening for that client, getting the
			// output from the client and then getting the inputstream for the client
			sel.select();

			Set<SelectionKey> keys = sel.selectedKeys();
			Iterator<SelectionKey> it = keys.iterator();
			while (it.hasNext()) {
				SelectionKey key = it.next();
				if (key.isAcceptable()) {
					it.remove();
					SocketChannel sc = serverSocket.accept();
					pool.execute(new Connections(sc, this));
				}
			}
			//server is only location where we only have one of these
			//need to pass the server object into the WebSocketResponse to have access to it
			//get the room and then enter while loop in WebSocket Response
			
			
			
			
			
			// assigning input and output lines. reading in each line.
			// splitting the line up into an array of lines
			// grabbing element 1 within the lines

			// Thread thread = new Thread(new Connection(clientSocket));
			// thread.start();
		}
		
	}
	public void getRoom(String roomProvided, WebSocketResponse responsePassed) throws SQLException, EOFException, IOException {
		String roomName = roomProvided; 
		
		if (roomObjects.containsKey(roomName)) {
		roomObjects.get(roomName).addUser(responsePassed);		
		responsePassed.setRoom(roomObjects.get(roomName));
		
		
		
		} else {
			Room newRoom = new Room(roomProvided, responsePassed, connect); 
			roomObjects.put(roomName, newRoom); 
			responsePassed.setRoom(newRoom);
		}
	}
}
