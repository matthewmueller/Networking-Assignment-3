#include <string>
#include <iostream>
#import "topology.hpp"
#import "utilities.hpp"
#import "socket.hpp"
// #import "packet.hpp"

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
	Node me = topology.getNode(host, port);
	cout << me.print();
	// Topology.refresh();
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
	
	Socket listener = Socket();
	listener.block(false);
	
	while(1) {
		string response = listener.listen(port);
		if(!response.empty()) {
			// Handle
		}
		
		createroutes();
		
		cout << "exit out of loop for now" << endl;
		exit(1);
	}
	
	
	
	return 0;
}
