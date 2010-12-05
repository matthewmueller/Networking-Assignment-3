#include <string>
#include <iostream>
#import "topology.hpp"
#import "utilities.hpp"
#import "socket.hpp"
#import "packet.hpp"

#include <vector>

using namespace std;

Topology topology;
string filename = "";
string host = "";
int port = -1;

void readtopology() {
	
	topology = Topology(filename);
}

void createroutes() {
	
}

void forwardpacket() {
	
}

int main (int argc, char const *argv[])
{
	// Get args
	filename = getArg("-f", argv);
	port = atoi(getArg("-p", argv).c_str());
	host = getMyHost();
	
	if(filename.empty()) {
		cout << "Need to specify a filename" << endl;
		exit(0);
	} else if(port < 1024 || port > 65535) {
		cout << "Invalid port" << endl;
		exit(0);
	}
	
	// Parse and generate topology
	readtopology();
	cout << getMyHost() << endl;
	
	// topology.getNode(host, port)
	Socket listener = Socket();
	listener.block(false);
	
	while(1) {
		string response = listener.listen(port);
		if(!response.empty()) {
			// Handle
		}
		
		createroutes();
	}
	
	
	
	return 0;
}