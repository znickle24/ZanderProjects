package serverHTTP;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.Socket;
import java.util.HashMap;
import java.util.Scanner;

public class HTTPRequest {
	private File myFile;
	private InputStream input = null;
	private String[] keyRequest = null;
	private Boolean isWebSocket = false;
	private String WSKey;
	private HashMap<String, String> map = new HashMap<>();

	public HTTPRequest(Socket clientSocket) throws BadRequestException {

		try {
			input = clientSocket.getInputStream();
		} catch (IOException e) {
			e.printStackTrace();
			throw new BadRequestException();
		}
		Scanner in = new Scanner(input);
		String inputline;
		inputline = in.nextLine();
		String[] lineRequest = inputline.split(" ");
		String firstWordRequest = lineRequest[0];
		String lastWordRequest = lineRequest[lineRequest.length - 1];
		if (!(lineRequest.length == (3)) || !firstWordRequest.equals("GET")) {
			throw new BadRequestException();
		} else if (!lastWordRequest.equals("HTTP/1.1")) {
			throw new BadRequestException();
		}

		String pathToFile = "Resources/" + lineRequest[1];
		myFile = new File(pathToFile);

		HashMap<String, String> map = new HashMap<>();
		while (true) {
			inputline = in.nextLine();

			if (inputline.equals("")) {
				break;
			}

			else {
				keyRequest = inputline.split(": ");
				map.put(keyRequest[0], keyRequest[1]);
			}
		}

		WSKey = map.get("Sec-WebSocket-Key");
		System.out.println(WSKey);
		if (map.containsKey("Sec-WebSocket-Key")) {
			isWebSocket = true;
		}
		if (myFile.exists()) {

			// while (!in.nextLine().equals("")) {
			// //does nothing
			// }
		}
	}

	public File getFile() throws Exception {

		return myFile;
	}

	public Boolean isWebSocket() {
		return isWebSocket;
	}

	public HashMap<String, String> getMap() {
		return map;

	}

	public String getWSKey() {
		return WSKey;
	}

}
