package serverProxy;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Server {
	private int portNumber = 8080; 
	private ServerSocketChannel serverSocket;
	private SocketChannel clientSocket;
	private Selector sel;
	public Server() throws IOException {
		serverSocket = ServerSocketChannel.open().bind(new InetSocketAddress(portNumber));
		serverSocket.configureBlocking(false);
		sel = Selector.open();
		serverSocket.register(sel, SelectionKey.OP_ACCEPT);
	}
	
public void serve() throws IOException, BadRequestException, NotImplementedException {
	ExecutorService pool = Executors.newFixedThreadPool(10);
	while (true) {
		sel.select(); 
		Set<SelectionKey> keys = sel.selectedKeys(); 
		Iterator<SelectionKey> it = keys.iterator(); 
		while (it.hasNext()) {
			SelectionKey key = it.next(); 
			if (key.isAcceptable()) {
				it.remove();
				clientSocket = serverSocket.accept();
				pool.execute(new HTTPRequest(clientSocket)); 
			}
		}
	}
}
}
