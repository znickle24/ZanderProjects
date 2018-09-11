package sslServer;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;
import java.util.Iterator;
import java.util.Set;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

public class Server {
	private int portNumber; 
	private ServerSocketChannel serverSocket; 
	private Selector sel; 
	private SocketChannel client; 
	private Certificate serverCert; 
	private PublicKey publicKey;
	private SecureRandom nonce; 
	private ServerClient serverClient; 
	public Server(int _portNumber) throws IOException, CertificateException, InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException {
		portNumber = _portNumber; 
		serverSocket = ServerSocketChannel.open().bind(new InetSocketAddress(portNumber));  
		serverSocket.configureBlocking(false); 
		sel = Selector.open(); 
		serverSocket.register(sel,  SelectionKey.OP_ACCEPT);
	}
	public void serve() throws IOException, InvalidKeyException, CertificateException, NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeySpecException, IllegalBlockSizeException, BadPaddingException {
		ExecutorService pool = Executors.newFixedThreadPool(100); 
		while (true) {
			sel.select(); 
			Set<SelectionKey> keys = sel.selectedKeys();
			Iterator<SelectionKey> it = keys.iterator(); 
			while (it.hasNext()) {
				client = serverSocket.accept();
				try {
					serverClient = new ServerClient(client);
				} catch (NullPointerException e) {
					break; 
				}
			}
		}
	}
}
