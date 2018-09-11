package sslServer;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.net.Socket;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;

public class ClientUtil  {
	private Socket clientSocket; 
	private InputStream key, cert, inFromServer; 
	private String inputLine, outputLine; 
	private Cipher c ; 
	private PrivateKey clientPrivateKey; 
	private PublicKey serverPubKey; 
	private byte[] r1, r2, toSha1, toSha2, message1ka, shaHash, shaHash2;
	private Certificate serverCert; 
	private SecureRandom nonce; 
	private byte[] masterSecret, decryptedFile;
	private Client client; 
	private SecretKey bobsAuthKey, alicesAuthKey, bobsEncKey, alicesEncKey;
	public ClientUtil(Socket _clientSocket, Client _client) throws IOException {
		client = _client;
		clientSocket = _clientSocket;
		inFromServer = clientSocket.getInputStream(); 
		PrintWriter outToServer = new PrintWriter(clientSocket.getOutputStream(), true); 
	}
	public void decryptR1(byte [] _ka, PrivateKey _clientPk) throws NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IllegalBlockSizeException, BadPaddingException {
		byte [] kaR1 = _ka; 
		clientPrivateKey = _clientPk;
		r1 = new byte[20];
		c = Cipher.getInstance("RSA"); 
		c.init(Cipher.DECRYPT_MODE, clientPrivateKey);
		r1 = c.doFinal(kaR1);
		System.out.println("Got to R1 decryption. I've verified they match");
	}
	public void encryptR2(Certificate _serverCert) throws CertificateException, NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IOException {
		System.out.println("Got to encrypt R2 beginning");
		serverCert = _serverCert;
		serverPubKey = serverCert.getPublicKey(); 
		nonce = new SecureRandom(); 
		r2 = new byte [20];
		nonce.nextBytes(r2);
		c = Cipher.getInstance("RSA");
		c.init(Cipher.ENCRYPT_MODE,  serverPubKey);
		System.out.println("Encryption of R2 end");
		byte[] kbR2 = null; 
		try {
			kbR2 = c.doFinal(r2);
		} catch (IllegalBlockSizeException | BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		assert(kbR2 != null);
		sendR2(kbR2);
	}
	public void sendR2(byte[] _kbR2) throws IOException, CertificateEncodingException, NoSuchAlgorithmException, NoSuchPaddingException {
		OutputStream outToServer = clientSocket.getOutputStream();
		try {
			outToServer.write(_kbR2);
		System.out.println("Got to sending the encoded R2 from Client");
		outToServer.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//for comparison of the hash that the server sent to me. 
		calcFirstHash(r1, r2);
	}
	public void CreateMasterSecret() {
		masterSecret = new byte[r1.length];
		for (int i = 0; i < r1.length; i++) {
			masterSecret[i] = (byte) (r1[i] ^ r2[i]);
			assert(masterSecret != null);
		}
	}
	public void calcFirstHash(byte[] _r1, byte[] _r2) throws CertificateEncodingException, NoSuchAlgorithmException, IOException, NoSuchPaddingException {
		message1ka = client.getMessage1();
		CreateMasterSecret();
		int len = 0; 
		toSha1 = new byte[message1ka.length + r1.length + r2.length + masterSecret.length];
		System.arraycopy(message1ka, 0, toSha1, 0, message1ka.length);
		len += message1ka.length;
		System.arraycopy(r1, 0, toSha1, len, r1.length);
		len += r1.length;
		System.arraycopy(r2, 0, toSha1, len, r2.length);
		len += r2.length;
		System.arraycopy(masterSecret, 0, toSha1, len, masterSecret.length);
		BigInteger sha = new BigInteger(toSha1);
		System.out.println(sha);
		MessageDigest sha1 = MessageDigest.getInstance("SHA-1");
		shaHash = sha1.digest(toSha1);
		if (GetHash()) {
			calcSecondHash();
		}
		
	}
	public void calcSecondHash() throws NoSuchAlgorithmException, IOException, NoSuchPaddingException {
		int len = 0; 
		toSha2 = new byte[message1ka.length + r1.length + r2.length + toSha1.length + masterSecret.length];
		System.arraycopy(message1ka, 0, toSha1, 0, message1ka.length);
		len += message1ka.length;
		System.arraycopy(r1, 0, toSha2, len, r1.length);
		len += r1.length;
		System.arraycopy(r2, 0, toSha2, len, r2.length);
		len += r2.length;
		System.arraycopy(toSha1, 0, toSha2, len, toSha1.length);
		len += toSha1.length;
		System.arraycopy(masterSecret, 0, toSha2, len, masterSecret.length);
		BigInteger sha = new BigInteger(toSha2);
//		System.out.println(sha);
		MessageDigest sha2 = MessageDigest.getInstance("SHA-1");
		shaHash2 = sha2.digest(toSha2);
		SendHash(toSha2);
	}
	public void SendHash(byte[] _toSha1) throws NoSuchAlgorithmException, IOException, NoSuchPaddingException  {
		MessageDigest sha = MessageDigest.getInstance("SHA-1");
		//sha1 of the values {m1, m2, m3, m4, s}
		byte[] sendToClient = sha.digest(_toSha1);
		System.out.println("Sending the sha message digest");
		OutputStream out;
		try {
			out = clientSocket.getOutputStream();
			out.write(sendToClient);
			//per the protocol, sending server with the first hash to the client
			String server = "Server";
			byte[] sendServer = server.getBytes();
			out.write(sendServer);
			out.flush();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("Done with the handshake!");
		createKeys();
	}
	public void createKeys() throws NoSuchAlgorithmException, IOException, NoSuchPaddingException {
		//these keys can be generated at both the client and server if the secure random is seeded with the ms

		SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
		random.setSeed(masterSecret); // s is the master secret
		KeyGenerator keyGenerator = KeyGenerator.getInstance("DESede");
		keyGenerator.init(random);
		bobsAuthKey = keyGenerator.generateKey();
		alicesAuthKey = keyGenerator.generateKey();
		bobsEncKey = keyGenerator.generateKey();
		alicesEncKey = keyGenerator.generateKey();
		System.out.println("Created all the keys");
		if (readMessage()) {
			System.out.println("Message Read In Successfully");
		} else {
			System.out.println("File Transfer Failed");
		}
	}
	public boolean GetHash() throws IOException {
		//compares the message digest of via sha-1 of the same information. 
		//needs to match in order to continue in the exchange
		byte[] hash = new byte[shaHash.length]; 
		inFromServer.read(hash);
		assert(hash != null);
		for (int i = 0; i < hash.length; i++) {
			if (hash[i] != shaHash[i]) {
				System.out.println("Hashes don't match. Try again.");
				return false; 
			}
		}
		String server = "Server";
		byte[] serverBytes = server.getBytes();
		byte[] sBytesLength = new byte[serverBytes.length];
		inFromServer.read(sBytesLength);
		System.out.println("Great! The hashes match.");
		return true;
	}

	public boolean readMessage() throws IOException, NoSuchAlgorithmException, NoSuchPaddingException {
		boolean read = false; 
		DataInputStream dis = new DataInputStream(clientSocket.getInputStream());
		Cipher c3 = Cipher.getInstance("DESede");
		try {
			c3.init(Cipher.DECRYPT_MODE, bobsEncKey);
		} catch (InvalidKeyException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		byte[] toBeDecrypted = new byte[dis.readInt()];
		dis.read(toBeDecrypted);
		try {
			decryptedFile = c3.doFinal(toBeDecrypted);
		} catch (IllegalBlockSizeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (decryptedFile != null) {
			FileOutputStream fos = new FileOutputStream(new File ("fileReadIn.txt"));
			fos.write(decryptedFile);
			fos.flush();
			fos.close();
			return true;
		}else {
			return false;
		}
	}
}
