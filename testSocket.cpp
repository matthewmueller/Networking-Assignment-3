#include "socket.hpp";

int main (int argc, char const *argv[])
{
	
	Socket socket = Socket();
	
	// Send to the localhost on port 9000 the message Hello World!....
	socket.send("localhost", 9000, "Hello World! Well this is just great isn't it? WhaooO!!!\n");
	socket.send("localhost", 9000, "Hello2!!!\n");
	socket.send("localhost", 9000, "Hello3\n");
	
	socket.send("localhost", 9000, "exit");
	
	// Disconnect the socket immediately after
	socket.disconnect();
	socket.reconnect();

	// socket.bind(12000);
	string response = socket.listen(12000);
	
	cout << response;
	
	return 0;
}