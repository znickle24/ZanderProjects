package serverHTTP;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.nio.channels.Channels;
import java.nio.channels.Pipe;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.sql.SQLException;
import java.util.Base64;
import java.util.Iterator;
import java.util.Set;

public class WebSocketResponse {
	private OutputStream output = null;
	private String decodedMessageText;
	private SocketChannel clientSocket; 
	private Server server; 
	private Pipe pipe; 
	private Room room; 
	public WebSocketResponse(SocketChannel clientSocket, Server serverPassed) throws Exception {
		this.clientSocket = clientSocket; 
		output = clientSocket.socket().getOutputStream();
		server = serverPassed; 
		
	}
	public WebSocketResponse() {
		
	}

	public void handshakeReply(String WSKey, SocketChannel clientSocket)
			throws NoSuchAlgorithmException, NoSuchProviderException, IOException, ClassNotFoundException {
		// testing to make sure that the system made it to the handshake method
		System.out.println("got to the handshake");
		String keyResponse = (WSKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
		MessageDigest md = MessageDigest.getInstance("SHA-1");
		String mdAsB64 = Base64.getEncoder().encodeToString(md.digest(keyResponse.getBytes()));
		// the send response method creates the handshake response and locks in the
		// connection
		sendResponse(mdAsB64);
		
		// once the handshake is complete, we need to call the echo.
		// the echo contains a method for sending the text back to the client who sent
		// the message.
	
		
		// server is connected and now we're listening for a message from the client

	}

	// the echo method currently does not support a room as one of the items the
	// selector can listen for.
	// once the room class has been added, need to make sure that the selector can
	// acknowledge hearing the room

	public void echo(SocketChannel clientSocket) throws IOException, ClassNotFoundException, SQLException {
		
		
		//need to decide which way to go -- room or response
		String requestedNameAndRoom = this.readMessage();
		String[] requestSplit = requestedNameAndRoom.split(" ");
		String usernameRequested = requestSplit[0]; 
		System.out.println("Did this get passed from the app" + usernameRequested);
		String roomRequested = requestSplit[1];
		System.out.println(usernameRequested);
		System.out.println(roomRequested);
		
		
		
		//parse info, store room name
		//use server getter for room object 
		//register the pipe with the selector right after the clientSocket.register(sel, SelectionKey.op_read); 
		pipe = Pipe.open(); 
		server.getRoom(roomRequested, this); 
		Selector sel = Selector.open();
		clientSocket.configureBlocking(false);
		clientSocket.register(sel, SelectionKey.OP_READ);
		pipe.source().configureBlocking(false);
		pipe.source().register(sel, SelectionKey.OP_READ);
		
		while (!(clientSocket.socket().isClosed())) {
			
			// clientSocketChannel is listening to see which code to run.
			// The end goal of this is to have the selector listen for a room or a client
			// action
			// have methods for what to do when the client talks to us, need to add for what
			// to do if the room talks to us
			sel.select();
			Set<SelectionKey> keys = sel.selectedKeys();
			Iterator<SelectionKey> it = keys.iterator();
			while (it.hasNext()) {
				SelectionKey key = it.next();
				Messages message; 
				if (key.isReadable()) {
					if (key.channel()==clientSocket) {
						clientSocket.keyFor(sel).cancel();
						clientSocket.configureBlocking(true);
						decodedMessageText = readMessage();
						int messageIndex = decodedMessageText.indexOf(" ");
						String username = decodedMessageText.substring(0, messageIndex); 
						String messageFinal = decodedMessageText.substring(messageIndex, decodedMessageText.length()); 
						System.out.print("The room shows as: " +roomRequested+"The username shows as: "+username+"The message shows as: " + messageFinal);
						message = new Messages (roomRequested, username, messageFinal); 
						room.postMessagetoRoom(message);
						//re-setting the socket so that it can be ready to send ore messages and the program can listen for those messages
						clientSocket.configureBlocking(false);
						sel.selectNow();
						clientSocket.register(sel, SelectionKey.OP_READ);	
					} 
					//once a message is received from the room, the pipe and clientSocket work to get the message to the user in a JSON format
					if (key.channel()==pipe.source()) {
						pipe.source().keyFor(sel).cancel();
						clientSocket.keyFor(sel).cancel();
						pipe.source().configureBlocking(true);
						clientSocket.configureBlocking(true);
						ObjectInputStream readMessage = new ObjectInputStream(Channels.newInputStream(pipe.source())); 
						Messages messageObject = (Messages) readMessage.readObject(); 
						String finalMessage = messageObject.toJSON(); 
						//testing to make sure that the message is what I expect when it goes through the system and out to the user
						System.out.println(finalMessage);
						sendText(finalMessage, clientSocket);
						
						//re-setting the socket so that it can be ready to send ore messages and the program can listen for those messages
						clientSocket.configureBlocking(false);
						sel.selectNow(); 
						clientSocket.register(sel, SelectionKey.OP_READ); 
						//re-setting the pipe to listen for other responses from the room
						pipe.source().configureBlocking(false);
						sel.selectNow(); 
						pipe.source().register(sel, SelectionKey.OP_READ);	
						
					
						
					}
				}
			}
		}
	}
//	This method is a means of sending the message that the room received back to the webSocket to be printed out for the user on the web page
	public synchronized void sendMessageFromRoom(Messages messagePassed) throws IOException {
		System.out.println("You got to the message from room");
		ObjectOutputStream message = new ObjectOutputStream(Channels.newOutputStream(pipe.sink()));
		message.writeObject(messagePassed);
		message.flush(); 
	}

	// prints out the message to the client (or multiple clients if possible)
	public void sendText(String message, SocketChannel clientSocket) throws IOException {
		String messageRead = new String();
		messageRead = message;

		DataOutputStream outputMessage = new DataOutputStream(clientSocket.socket().getOutputStream());
		byte[] header;
//		System.out.println(messageRead + "send text version");
		if (messageRead.length() < 126) {
			header = new byte[2];
			header[0] = (byte) 0x81;
			header[1] = (byte) messageRead.length();
		} else {
			header = new byte[4];
			header[0] = (byte) 0x81;
			header[1] = 126;
			header[2] = (byte) (messageRead.length() / 256);
			header[3] = (byte) (messageRead.length() % 256);
			System.out.println(messageRead);
		}
		
		outputMessage.write(header);
		outputMessage.write(messageRead.getBytes());
		outputMessage.flush();
	}

	// sends the handshake response to the client in order to let it know that the
	// connection has been accepted
	public void sendResponse(String magicCode) {
		PrintWriter out = new PrintWriter(output);
		String mdAsB64 = new String();
		mdAsB64 = magicCode;
		out.print("HTTP/1.1 101 Switching Protocols" + "\r\n");
		out.print("Upgrade: websocket" + "\r\n");
		out.print("Connection: Upgrade" + "\r\n");
		out.print("Sec-WebSocket-Accept: " + mdAsB64 + "\r\n");
		out.print("\r\n");
//		System.out.println(out);
		out.flush();
	}

	public String readMessage() throws IOException, EOFException {
		System.out.println("got to the decodeMessage");
	
		DataInputStream data = new DataInputStream((clientSocket).socket().getInputStream());
		byte[] b = new byte[2];
		data.readFully(b);
		byte b1 = b[0];
		byte b2 = b[1];

		// correct when it comes through before the tests
		// System.out.println(opcode);
		// System.out.println(payloadLength);
		int dataLength;
		b1 = (byte) (b1 >> 4 & 0xF);
		if (b1 == 8) {
			// need to do something at this point since the value is what I am looking for
			System.out.println("You got it!");
			System.out.println(b1);
		}
		if ((b1 & 0xF) == 1) {
			// opcode
			// need to do something at this point - like move to
			System.out.println("Success!!");
			System.out.println(b1);
		}
		dataLength = (b2 & 0x7F);
		System.out.println(dataLength);
		if ((b2 >> 7 & 1) != 1) {
			System.out.println("bad connection");
			clientSocket.close();
		}
		if ((b2 >> 4 & 0xF) == 8) {
			// need to do something at this point - move to a function to perform an action
			System.out.println("Success");
		}
		if (dataLength < 126) {
			// done if this is the case - it's all going to be within the first byte

		} else if (dataLength == 126) {
			int i1 = data.readUnsignedByte();
			int i2 = data.readUnsignedByte();
			i1 = (i1 << 8);
			dataLength = (i1 + i2);

			System.out.println("Data Length Is: " + dataLength);

			// read the next 16 bits and interpret those as an unsigned integer
		} else {
			data.readLong();
			dataLength = (int) data.readLong();
			System.out.println("not working now");
			// read next 64 bits and interpret those as an unsigned integer
		}

		byte[] maskingKey = new byte[4];
		data.readFully(maskingKey);
		byte[] encodedData = new byte[dataLength];
		data.readFully(encodedData);

		byte[] messageDecoded = new byte[dataLength];

		for (int i = 0; i < dataLength; i++) {
			messageDecoded[i] = (byte) (encodedData[i] ^ maskingKey[i % 4]);
		}
		String decodedMessageTextWithUser = new String(messageDecoded);
		
	
		return decodedMessageTextWithUser;
	}
	public void setRoom(Room roomPassed) throws IOException {
		room = roomPassed; 
		room.getUpdatesForWhenIWasOut(this);
	}

	
}
