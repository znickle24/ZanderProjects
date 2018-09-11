package sslServer;

import java.io.IOException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

public class ServerDriver {
	public static void main(String[] args) throws InvalidKeyException, CertificateException, NoSuchAlgorithmException, NoSuchPaddingException, IOException, InvalidKeySpecException, IllegalBlockSizeException, BadPaddingException {
		Server server = new Server(8080);
		server.serve(); 
	}
}
