#include "socket.hpp";

int main (int argc, char const *argv[])
{
	
	Socket socket = Socket();
	socket.block(false);
	// socket.bind(9000);
	
	string response;
	while(1) {
		response = socket.listen(9000);
		if(!response.empty()) {
			if(response.compare("exit") == 0) {
				break;
			} else {
				cout << response << endl;				
			}
		} 
	}
	// cout << response << endl;
	socket.send("localhost", 12000, "Hello From Listener!");
	// cout << socket.getProtocol();
	return 0;
}