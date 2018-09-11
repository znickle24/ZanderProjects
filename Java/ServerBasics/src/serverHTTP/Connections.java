package serverHTTP;

import java.io.File;
import java.net.Socket;
import java.nio.channels.SocketChannel;

public class Connections implements Runnable {
	private SocketChannel clientSocket;
	private Server server; 
	private Room room; 
	
//	public static ExecutorService new FixedThreadPool(int 100) {
//	}
	public Connections (SocketChannel clientSocket, Server server) {
		this.clientSocket = clientSocket; 
		this.server = server; 
		 
	}
	
	@Override
	public void run() {
	
		try {
		
		HTTPRequest returnFile = null;
		returnFile = new HTTPRequest (clientSocket.socket());
		File myFile = returnFile.getFile();
		if (!(returnFile.isWebSocket())) {
		
		
		
		//testing to see where I'm looking 
		String absolutePath; 
		absolutePath = myFile.getAbsolutePath();
		System.out.println(absolutePath);
		
		HTTPResponse response = new HTTPResponse (clientSocket.socket()); 
		response.sendResponse(myFile); 
		
		} else {
			WebSocketResponse response = new WebSocketResponse(clientSocket, server); 
			response.handshakeReply(returnFile.getWSKey(), clientSocket);
			
			response.echo(clientSocket);
			System.out.println("WS Test Successful");
			System.out.println(response);
		}
		
		clientSocket.close(); 
		
	} catch (BadRequestException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	} catch ( Exception e) {
		e.printStackTrace();
	}
	}
	
}
