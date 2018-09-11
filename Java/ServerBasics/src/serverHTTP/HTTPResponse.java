package serverHTTP;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class HTTPResponse {
	private OutputStream output = null;

	public HTTPResponse(Socket clientSocket) throws Exception {
		output = clientSocket.getOutputStream();
	}

	public void sendResponse(File myFile) throws IOException, InterruptedException {

		PrintWriter out = new PrintWriter(output);
		if (myFile.exists()) {
			out.print("HTTP/1.1 200 OK\r\n");
			out.print("Content-Length: " + myFile.length() + "\r\n");
			out.print("\r\n");
			out.flush();

			FileInputStream inStream = new FileInputStream(myFile);
			byte[] bis = new byte[1024];
			int bytes = inStream.read(bis);
			do {
				output.write(bis, 0, bytes);
				bytes = inStream.read(bis);
				output.flush();
				Thread.sleep(10);
			} while (bytes > 0);
			inStream.close();

		} else {
			out.print("HTTP/1.1 404 ERROR\n");
			out.flush();
		}
		out.close();
	}

}
