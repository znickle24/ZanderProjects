package sslServer;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.math.BigInteger;
import java.net.Socket;
import java.nio.channels.SocketChannel;
import java.security.InvalidKeyException;
import java.security.KeyFactory;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.util.Scanner;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;

public class ServerUtil {
	private Socket clientSocket; 
	private InputStream clientInput; 
	private PublicKey clientPubKey;
	private PrivateKey serverPrivKey;
	private Certificate clientCert, serverCert; 
	private SecureRandom nonce;
	private byte[] r1, r2, shaHash2, masterSecret, message1ka, toSha1, toSha2; 
	private Cipher c, c2;
	private SecretKey bobsAuthKey, alicesAuthKey, bobsEncKey, alicesEncKey;
	public ServerUtil(SocketChannel _clientSocket) {
		clientSocket = _clientSocket.socket();
		try {
			clientInput = clientSocket.getInputStream();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} 
	}
	public void EncryptClientInfo(InputStream clientInput) throws CertificateException, NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IOException, InvalidKeySpecException, IllegalBlockSizeException, BadPaddingException {
		//generate the client certificate from the inputstream and grab the public key from that
		CertificateFactory cf = CertificateFactory.getInstance("X.509");
		clientCert = cf.generateCertificate(clientInput);
		System.out.println("Got certificate from client");
		clientPubKey = clientCert.getPublicKey(); //Ka+ for Server to encrypt with R1, a nonce
		message1ka = clientPubKey.getEncoded();
		nonce = new SecureRandom(); 
		r1 = new byte [20];
		nonce.nextBytes(r1);
		c = Cipher.getInstance("RSA"); 
		c.init(Cipher.ENCRYPT_MODE, clientPubKey);
		System.out.println("Initialized Encryption");
		byte [] kaR1 = null;
		try {
			//encrypt r1 with Ka
			kaR1 = c.doFinal(r1);
		} catch (IllegalBlockSizeException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (BadPaddingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		assert(kaR1 != null);
		serverCert = cf.generateCertificate(new FileInputStream ("sslCertSigned.cert"));
		//this will send the cert first and then R1 encrypted with ka+
		SendCert(serverCert, kaR1); 
		byte[] kbR2 = new byte[256];
		//grab the information from the client input stream 
		clientInput.read(kbR2);
		//use kb- to decrypt R2
		DecryptR2(kbR2);
	}
	public void SendCert(Certificate _serverCert, byte[] _kaR1) throws IOException {
		OutputStream out = clientSocket.getOutputStream(); 
		try {
			out.write(_serverCert.getEncoded());
			out.write(_kaR1);
			System.out.println("Got to sending the encoded R1 from Server");
			out.flush(); 
		} catch (CertificateEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	public void DecryptR2(byte [] _kbR2) throws IOException, NoSuchAlgorithmException, InvalidKeySpecException, NoSuchPaddingException, InvalidKeyException, BadPaddingException, IllegalBlockSizeException {
		File pkFile = new File("prv.der");
		FileInputStream fis = new FileInputStream(pkFile); 
		byte [] pkBA = new byte[(int) pkFile.length()];
		fis.read(pkBA);
		KeyFactory kf = KeyFactory.getInstance("RSA"); 
		PKCS8EncodedKeySpec pkcs8 = new PKCS8EncodedKeySpec (pkBA);
		serverPrivKey = kf.generatePrivate(pkcs8);
		byte [] kbR2 = _kbR2;
		c2 = Cipher.getInstance("RSA");
		c2.init(Cipher.DECRYPT_MODE, serverPrivKey);
		r2= c2.doFinal(kbR2);
		System.out.println(r1.toString());
		System.out.println(r2.toString());
		System.out.println("Got to R2 decryption. Have verified it matches from Client.");
		assert(r1 != null);
		assert(r2 != null);
		//just r1 XOR r2
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
//		System.out.println(sha);
		//generates message digest and sends to client
		SendHash(toSha1);
		calcHash2();
	}
	//needs access to message 1 (certificate), message 2 (certificate and ka+R1), 
	//message 3 (kb+R2) and will send a secret message, 's'
	//will create sha1 of each of those messages combined. sha1 takes a byte array. 
	public void SendHash(byte[] _toSha1) throws NoSuchAlgorithmException  {
		MessageDigest sha = MessageDigest.getInstance("SHA-1");
		//sha1 of the values {m1, m2, m3, s}
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
	}
	public void calcHash2() throws NoSuchAlgorithmException, IOException, InvalidKeyException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException {
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
		System.out.println(sha);
		MessageDigest sha1 = MessageDigest.getInstance("SHA-1");
		shaHash2 = sha1.digest(toSha2);
		if (GetHash()) {
			createKeys();
			sendMessage();
		}
	}
	public void createKeys() throws NoSuchAlgorithmException, IOException, InvalidKeyException, NoSuchPaddingException, IllegalBlockSizeException, BadPaddingException {
		SecureRandom random = SecureRandom.getInstance("SHA1PRNG");
		random.setSeed(masterSecret); // s is the master secret

		KeyGenerator keyGenerator = KeyGenerator.getInstance("DESede");
		keyGenerator.init(random);

		bobsAuthKey = keyGenerator.generateKey();
		alicesAuthKey = keyGenerator.generateKey();
		bobsEncKey = keyGenerator.generateKey();
		alicesEncKey = keyGenerator.generateKey();
		sendMessage();
	}
	public void sendMessage() throws IOException, NoSuchAlgorithmException, NoSuchPaddingException, InvalidKeyException, IllegalBlockSizeException, BadPaddingException {
		File file = new File ("textFile.txt");
		FileInputStream fis = new FileInputStream(file);
		byte[] fileBArray = new byte[(int) file.length()];
		fis.read(fileBArray);
		DataOutputStream dos = new DataOutputStream(clientSocket.getOutputStream());
		Cipher c3 = Cipher.getInstance("DESede");
		c3.init(Cipher.ENCRYPT_MODE, bobsEncKey);
		byte[] encryptedMessage = c3.doFinal(fileBArray);
		dos.writeInt(encryptedMessage.length);
		dos.write(encryptedMessage); 
		dos.flush();
		//byte array 
	}
	public void CreateMasterSecret() {
		System.out.println("In the master secret creation");
		masterSecret = new byte[r1.length];
		for (int i = 0; i < r1.length; i++) {
			masterSecret[i] = (byte) (r1[i] ^ r2[i]);
		}
		System.out.println("Got to the end of creating the master secret");
	}
	public boolean GetHash() throws IOException {
		//compares the message digest of via sha-1 of the same information. 
		//needs to match in order to continue in the exchange
		byte[] hash = new byte[shaHash2.length]; 
		InputStream in = clientSocket.getInputStream();
		in.read(hash);
		assert(hash != null);
		//compare each item in the hash with the other hash to make sure I'm speaking to the correct client
		for (int i = 0; i < hash.length; i++) {
			if (hash[i] != shaHash2[i]) {
				//per protocol, deny any client that sends a hash that is incorrect
				System.out.println("Hashes don't match. Try again.");
				return false; 
			}
		}
		System.out.println("Great! The hashes match.");
		return true;
	}

}
