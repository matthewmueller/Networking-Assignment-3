#include <string>
#include <iostream>
#include "topology.hpp"
#include "utilities.hpp"
#include "socket.hpp"
#include "packet.hpp"

using namespace std;

Topology topology;
string filename = "";
int port = -1;

void readtopology() {
	
	string contents = file_get_contents(filename);
	
	cout << contents;
	
}

int main (int argc, char const *argv[])
{
	// Get args
	string filename = getArg("-f", argv);
	int port = atoi(getArg("-p", argv).c_str());
	
	if(filename.empty()) {
		cout << "Need to specify a filename" << endl;
		exit(0);
	} else if(port < 1024 || port > 65535) {
		cout << "Invalid port" << endl;
		exit(0);
	}
	
	// Parse and generate topology
	readtopology();
	
	return 0;
}