#include <string>
#include <iostream>
#include <map>
#import "topology.hpp"
#import "utilities.hpp"
#import "socket.hpp"
#import "ackpacket.hpp"
#import "tracepacket.hpp"
#import "linkstatepacket.hpp"
// #import "packet.hpp"

#include <vector>

using namespace std;

Topology topology;
string filename = "";
string host = "";
Node me;
int port = -1;

void readtopology() {
	
	topology = Topology(filename);
}

// Refresh the topology by using the Reliable Flooding algorithm
void createroutes() {

	// Begin the flood starting at my node.
	cout << "called create routes";
	
}

void forwardpacket() {
	
}

int main (int argc, char const *argv[])
{
	// Get args
	filename = getArg("-f", argv);
	port = atoi(getArg("-p", argv).c_str());
	host = getMyIP();
		
	if(filename.empty()) {
		cout << "Need to specify a filename" << endl;
		exit(0);
	} else if(port < 1024 || port > 65535) {
		cout << "Invalid port" << endl;
		exit(0);
	}
	
	// Parse and generate topology
	readtopology();
	
	// Get my Node representation from Topology
	Node me = topology.getNode(host, port);
		
	Socket listener = Socket();
	listener.block(false);
	
	// Start the timer.
	long long startTime = getMilliTime();
	int elapsedTime = 0;
	while(1) {
		string response = listener.listen(port);
		if(!response.empty()) {
			// It's going to either be a linkstatepacket, tracepacket, or ackpacket
			vector<string> packParts = explode(" ", response);
			string type = packParts[0];
			
			if(type.compare("T") == 0) {
				// trace packet
				
			} else if(type.compare("A") == 0) {
				// ack packet - we know one of our neighbors is online. Should only occur when we run createroutes.
				
			} else {
				// Should be a linkstatepacket
				
			}
			
		}

		elapsedTime = getElapsedTime(startTime);
		
		// QUESTION: SHOULD ALL EMULATORS BE CREATING ROUTES OR DOES IT
		// ONLY NEED TO ORIGINATE FROM ONE EMULATOR
		
		// Run create routes after 5 seconds - probably should be random,
		// if they all are to be sent out to avoid clogging.
		if(elapsedTime > 5000) {
			createroutes();
			startTime = getMilliTime();
			exit(1);
		}
		
		// cout << "exit out of loop for now" << endl;
		// 		exit(1);
	}
	
	
	
	return 0;
}
