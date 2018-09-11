package sslServer;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.channels.SocketChannel;
import java.security.InvalidKeyException;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.spec.InvalidKeySpecException;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

public class ServerClient {
	private SocketChannel clientSocket; 
	private Certificate serverCert; 
	private PublicKey serverPubKey;
	private PrivateKey serverPrivKey;
	private OutputStream out;
	private InputStream in;
	public ServerClient(SocketChannel client) throws IOException, CertificateException, InvalidKeyException, NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeySpecException, IllegalBlockSizeException, BadPaddingException {
		clientSocket = client;
		CertificateFactory cf = CertificateFactory.getInstance("X.509");
		serverCert = cf.generateCertificate(new FileInputStream ("sslCertSigned.cert"));
		serverPubKey = serverCert.getPublicKey();
		OutputStream out;
		try {
			out = clientSocket.socket().getOutputStream(); 
			in = clientSocket.socket().getInputStream();
		} catch (NullPointerException e) {
			clientSocket.close();
		}
		KeyFactory kf = KeyFactory.getInstance("RSA"); 
		ServerUtil util = new ServerUtil(clientSocket); 
		util.EncryptClientInfo(in); 
	}
}
