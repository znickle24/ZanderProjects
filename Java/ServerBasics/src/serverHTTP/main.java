package serverHTTP;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Scanner;

public class main {

	public static void main(String[] args) throws Exception {

	Server serverObject = new Server (); 
		serverObject.serve();
		
	}
}
