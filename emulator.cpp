#include <string>
#include <iostream>
#include <map>
#import "topology.hpp"
#import "utilities.hpp"
#import "socket.hpp"
#import "ackpacket.hpp"
#import "tracepacket.hpp"
#import "linkstatepacket.hpp"
#import "timer.hpp"
// #import "packet.hpp"

#include <vector>

using namespace std;

Topology topology;
string filename = "";
string host = "";
Node me;
vector<Node> acksNeededFromNeighbors;
int port = -1;
Timer timer = Timer();

void readtopology() {
	
	topology = Topology(filename);
}

// Refresh the topology by using the Reliable Flooding algorithm
void createroutes() {
	acksNeededFromNeighbors = me.neighbors();
	// Begin the flood starting at my node.
	cout << "called create routes";
	
}

void forwardpacket(string response) {
	// It's going to either be a linkstatepacket, tracepacket, or ackpacket
	vector<string> packParts = explode(" ", response);
	string type = packParts[0];
	
	if(type.compare("T") == 0) {
		// trace packet
		
	} else if(type.compare("A") == 0) {
		// ack packet - we know one of our neighbors is online. Should only occur when we run createroutes.
		
		if(acksNeededFromNeighbors.empty()) {
			// We got all our acks no need to disable a node in our topology. or send in the fire squad (ie. run createroutes)
		}
		
	} else {
		// Should be a linkstatepacket - forward it on to all neighbors EXCEPT the one we recieved it from
		// After its sent out we should start receiving ack packets from our neighbors
		acksNeededFromNeighbors = me.neighbors(); // NOTE: This should not be re
	}
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
	me = topology.getNode(host, port);
	
	
	Socket listener = Socket();
	listener.block(false);
	
	// Start the timer.
	long long startTime = getMilliTime();
	int elapsedTime = 0;
	while(1) {
		string response = listener.listen(port);
		if(!response.empty()) {
			forwardpacket(response);
		}

		elapsedTime = getElapsedTime(startTime);
		
		// ---- OK I THINK I FIGURED IT OUT:
		
		/* Every little bit we send out "hello" (acks. perhaps..)
		   packets to our neighbors
			
			IF we don't get a response from the neighbor we declare that its offline and 
			send out the LSPacket from the node that found out theres an offline node. 
			The Neighbors field in the LSPacket should contain which node is OFFLINE.
			
			EVERY ONCE IN A WHILE we send out LSPackets, but just to make sure "nothing has changed"
		*/
		
		// Run createroutes on its own say.. 20 seconds
		if(elapsedTime > 20000) {
			createroutes();
			startTime = getMilliTime();
			exit(1);
		}
		
		// cout << "exit out of loop for now" << endl;
		// 		exit(1);
	}
	
	
	
	return 0;
}
