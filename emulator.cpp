#include <string>
#include <iostream>
#include <map>
#import "topology.hpp"
#import "utilities.hpp"
#import "socket.hpp"
#import "handshakepacket.hpp"
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
Timer ackTimer = Timer();
int port = -1;
Timer timer = Timer();
stringstream debug;

void readtopology() {
	
	topology = Topology(filename);
}

// Refresh the topology by using the Reliable Flooding algorithm
void createroutes() {
	// acksNeededFromNeighbors = me.neighbors();
	// Begin the flood starting at my node.
	cout << "called create routes" << endl;
	
}

void forwardpacket(string response) {
	// It's going to either be a linkstatepacket, tracepacket, or ackpacket
	vector<string> packParts = explode(" ", response);
	string type = packParts[0];
	Socket sock = Socket();
	if(type.compare("T") == 0) {
		// trace packet
		
	} else if(type.compare("A") == 0) {
		// ack packet - we know one of our neighbors is online. Should only occur when we run createroutes.
		HSPacket ack = HSPacket(response);
		
		// Debugging information
		cout << "Recieved ack packet from: " << ack.sourceIP() << " on port: " << ack.sourcePort() << endl;
		
		string destIP = ack.sourceIP();
		int destPort = ack.sourcePort();
		
		HSPacket response = HSPacket().type('R').sourceIP(host).sourcePort(port).destinationIP(destIP).destinationPort(destPort);
		
		sock.send(destIP, destPort, response.toString());
		
	} else if(type.compare("R") == 0) {
		// Here's we're getting responses from acks
		HSPacket r = HSPacket(response);
		
		// Debugging information
		cout << "Recieved response packet from: " << r.sourceIP() << " on port: " << r.sourcePort() << endl;
		
		string ackFromIP = r.sourceIP();
		int ackWithPort = r.sourcePort();
		
		stringstream ss;
		// Remove them from acksNeededFromNeighbors

		cout << debug.str();
		for(unsigned int i = 0; i < acksNeededFromNeighbors.size(); i++) {
			Node n = acksNeededFromNeighbors.at(i);
			if(ackFromIP.compare(n.host()) == 0 && ackWithPort == n.port()) {
				acksNeededFromNeighbors.erase(acksNeededFromNeighbors.begin()+i);
				ss.str("");
				ss << "A|" << n.host() << ":" << n.port();
				ackTimer.remove(ss.str());
			}
		}
		
		if(acksNeededFromNeighbors.empty()) {
			ackTimer = Timer();
			cout << "No more acks needed from neighbors! " << endl;
			// We got all our acks no need to disable a node in our topology. or send in the fire squad (ie. run createroutes)
		}
	} 
	else {
		// Should be a linkstatepacket - forward it on to all neighbors EXCEPT the one we recieved it from
		// After its sent out we should start receiving ack packets from our neighbors

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
	
	
	Socket sock = Socket();
	sock.block(false);
	
	// Start the timer.
	timer.add("messageNeighbors", 1000);
	timer.add("updateLSP", 20000);
	
	while(1) {
		string response = sock.listen(port);
		if(!response.empty()) {
			forwardpacket(response);
			response = string("");
		}

		if(timer.isDone("messageNeighbors")) {
			stringstream ss;
			acksNeededFromNeighbors = me.neighbors();

			for(unsigned int i = 0; i < acksNeededFromNeighbors.size(); i++) {
				Node n = acksNeededFromNeighbors.at(i);
				// Create the ack packet
				HSPacket ack = HSPacket().type('A').sourceIP(host).sourcePort(port).destinationIP(n.host()).destinationPort(n.port());
				
				// Create the timer
				ss.str("");
				ss << "A|" << n.host() << ":" << n.port();
				// Set the timer
				ackTimer.add(ss.str(), 500);
				
				// Send the ack packet to our neighbor
				sock.send(n.host(), n.port(), ack.toString());
			}
			timer.add("messageNeighbors", 1000);
		}
		
		// Check to see if expiration up on acksNeededFromNeighbors
		stringstream key;
		for(unsigned int i = 0; i < acksNeededFromNeighbors.size(); i++) {
			Node n = acksNeededFromNeighbors.at(i);
			key.str("");
			key << "A|" << n.host() << ":" << n.port();
			
			if(ackTimer.isDone(key.str())) {
				// This node did not respond. Disable in topology
				topology.disable(n);

				// Then trigger reliable flooding algorithm to update node topologies
				createroutes();
			}
 		}
		
		
		// ---- OK I THINK I FIGURED IT OUT:
		
		/* Every little bit we send out "hello" (acks. perhaps..)
		   packets to our neighbors
			
			IF we don't get a response from the neighbor we declare that its offline and 
			send out the LSPacket from the node that found out theres an offline node. 
			The Neighbors field in the LSPacket should contain which node is OFFLINE.
			
			EVERY ONCE IN A WHILE we send out LSPackets, but just to make sure "nothing has changed"
		*/
		
		// Run createroutes on its own say.. 20 seconds
		
		
		// cout << "exit out of loop for now" << endl;
		// 		exit(1);
	}
	
	
	
	return 0;
}
