package sslServer;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;
import java.security.InvalidKeyException;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;


public class Client {
	private Socket clientSocket;
	private Certificate clientCert, serverCert; 
	private PublicKey clientPubKey, serverPubKey;
	private PrivateKey clientPrivateKey; 
	public Client(int portNum) throws IOException, CertificateException, NoSuchAlgorithmException, InvalidKeySpecException, InvalidKeyException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException {
		File pkFile = new File("prv.der");
		FileInputStream fis = new FileInputStream(pkFile); 
		byte [] pkBA = new byte[(int) pkFile.length()];
		fis.read(pkBA); 
		KeyFactory kf = KeyFactory.getInstance("RSA"); 
		PKCS8EncodedKeySpec pkcs8 = new PKCS8EncodedKeySpec (pkBA);
		clientPrivateKey = kf.generatePrivate(pkcs8);
		InetAddress host = InetAddress.getLocalHost();
		clientSocket = new Socket(host, portNum); 
		OutputStream out = clientSocket.getOutputStream(); 
		CertificateFactory cf = CertificateFactory.getInstance("X.509");
		clientCert = cf.generateCertificate(new FileInputStream ("sslCertSigned.cert"));
		clientPubKey = clientCert.getPublicKey(); 
		out.write(clientCert.getEncoded());
		out.flush(); 
		InputStream in = clientSocket.getInputStream();
		serverCert = cf.generateCertificate(in);
		serverPubKey = serverCert.getPublicKey();
		byte[] kaR1 = new byte[256];
		in.read(kaR1); //need to decrypt now 
		ClientUtil clientUtil = new ClientUtil(clientSocket, this); 
		//gets back R1 from the encrypted byte[] sent by the server
		clientUtil.decryptR1(kaR1, clientPrivateKey);
		//encrypts R2 with kb+ and sends to the server
		clientUtil.encryptR2(serverCert);
		//grabs the hash and compares. If true, going to send the 2nd hash
	}
	public byte[] getMessage1() throws CertificateEncodingException {
		byte[] firstMessage = clientPubKey.getEncoded();
		return firstMessage;
		
	}
}
