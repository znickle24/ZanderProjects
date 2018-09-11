package serverProxy;

import java.io.IOException;

public class main {

	public static void main(String[] args) throws IOException, BadRequestException, NotImplementedException {
		// TODO Auto-generated method stub
	Server serverObj = new Server(); 
	serverObj.serve();
	}

}
