package serverProxy;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

public class HTTPRequest implements Runnable {
	private InputStream input = null;
	private OutputStream out = null; 
	private String WSKey;
	private SocketChannel clientSocket; 
	public HTTPRequest(SocketChannel _clientSocket)  {
		clientSocket = _clientSocket;
	}
	@Override
	public void run() {
			try {
				input = clientSocket.socket().getInputStream();
			} catch (IOException e) {
				e.printStackTrace();
				try {
					throw new BadRequestException();
				} catch (BadRequestException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
			Scanner in = new Scanner(input);
			String inputline;
			inputline = in.nextLine();
			String[] lineRequest = inputline.split(" ");
			String firstWordRequest = lineRequest[0];
			String websiteRequested = lineRequest[1]; 
			String lastWordRequest = lineRequest[lineRequest.length - 1];
			for (String word: lineRequest) {
				System.out.println("Word in request: " + word);
			}
			if (!firstWordRequest.equals("GET")) {
				if (firstWordRequest.equals("PUT") || firstWordRequest.equals("HEAD")|| 
						firstWordRequest.equals("POST")||firstWordRequest.equals("DELETE")
						||firstWordRequest.equals("CONNECT")||firstWordRequest.equals("OPTIONS")
						||firstWordRequest.equals("TRACE")) {
					try {
						throw new NotImplementedException();
					} catch (NotImplementedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} 
				}
			}
			
			if (!(lineRequest.length == (3))) {
				try {
					throw new BadRequestException();
				} catch (BadRequestException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			} else if (!(lastWordRequest.equals("HTTP/1.1")) && !(lastWordRequest.equals("HTTP/1.0"))) {
				System.out.println("The last word: " + lastWordRequest);
				try {
					throw new BadRequestException();
				} catch (BadRequestException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
//			websiteRequested = websiteRequested.replaceFirst("/", ""); 
			System.out.println(websiteRequested);
			URL url = null;
			try {
				url = new URL(websiteRequested);
			} catch (MalformedURLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			HttpURLConnection connect = null;
			try {
				connect = (HttpURLConnection) url.openConnection();
			} catch (IOException e2) {
				// TODO Auto-generated catch block
				e2.printStackTrace();
			}  
			connect.setDoInput(true);
			connect.setDoOutput(false);
			try {
				connect.setRequestMethod("GET");
			} catch (ProtocolException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			connect.setRequestProperty("CONNECTION", "close");
			String host = url.getHost(); 
			try {
				out = clientSocket.socket().getOutputStream();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
			int responseCode;
			try {
				responseCode = connect.getResponseCode();
				System.out.println(responseCode);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 

			InputStream inStream = null;
			try {
				inStream = connect.getInputStream();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			} 
			byte[] buffer = new byte[1024]; 
			int bufferSize = 0; 
			do {
					try {
						bufferSize = inStream.read(buffer);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} 
					if (bufferSize < 0) {
						break; 
					}
					try {
						out.write(buffer, 0, buffer.length);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} 
					System.out.println("Buffer is: " + buffer);
			} while (bufferSize > 0); 
			try {
				out.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
			try {
				inStream.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
			try {
				out.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
		}
	 }

