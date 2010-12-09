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
int sequenceNumber = 0;
map<string, int> LSPSequenceMap;

// void decrementTTL() {
// 	map<string, int>::iterator it;
// 	cout << "Decrementing TTL map" << endl;
// 	for(it = LSPSequenceMap.begin(); it != LSPSequenceMap.end(); ++it) {
// 		string key = (it)->first;
// 		int TTL = (it)->second;
// 		cout << " TTL : " << TTL << endl;
// 		TTL--;
// 		if(TTL >= 0) {
// 			LSPSequenceMap[key] = TTL;
// 		} else {
// 			LSPSequenceMap[key] = 0;
// 		}
// 		cout << LSPSequenceMap[key] << endl;
// 	}
// }

string toID(string host, int port) {
	stringstream ss; 
	ss << host << ":" << port;
	
	return ss.str();
}

map<string, string> fromID(string ID) {
	map<string, string> out;
	vector<string> parts = explode(":", ID);
	out["host"] = parts[0];
	out["port"] = parts[1];
	
	return out;
}

bool shouldUpdate(LSPacket LSP) {
	string LSPFrom = LSP.id();
	
	if(LSP.TTL() <= 0) {
		LSPSequenceMap[LSPFrom] = 0;
		return false;
	}
	
	if(LSPSequenceMap.count(LSPFrom)) {
		if(LSP.sequenceNumber() > LSPSequenceMap[LSPFrom]) {
			return true;
		} else {
			return false;
		}
	} else {
		LSPSequenceMap[LSPFrom] = LSP.sequenceNumber();
		return true;
	}
	
}

void readtopology() {
	
	topology = Topology(filename);
}

// Refresh the topology by using the Reliable Flooding algorithm
// Use TTL to eventually stop the algorithm
void createroutes(vector<Node>sendTo, int TTL) {
	// acksNeededFromNeighbors = me.neighbors();
	// Begin the flood starting at my node.
	if(sendTo.empty()) return;
	
	string ID = toID(me.host(), me.port());
	string neighbors = topology.getNeighborsForLSP(me);
	
	LSPacket LSP = LSPacket().id(ID).neighbors(neighbors).sequenceNumber(sequenceNumber).TTL(TTL);
	
	Socket sock = Socket();
	// Flood to the neighbors
	for(unsigned int i = 0; i < sendTo.size(); i++) {
		Node n = sendTo.at(i);
		sock.send(n.host(), n.port(), LSP.toString());
	}
	
	// Calculate the routes
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
		stringstream write;
		write << "Recieved ack packet from: " << ack.sourceIP() << " on port: " << ack.sourcePort();
		cout << colorize(write.str(), "green");
		
		string destIP = ack.sourceIP();
		int destPort = ack.sourcePort();
		
		HSPacket response = HSPacket().type('R').sourceIP(host).sourcePort(port).destinationIP(destIP).destinationPort(destPort);
		
		sock.send(destIP, destPort, response.toString());
		
		// Used to create reliable transfer
		sequenceNumber++;
		
	} else if(type.compare("R") == 0) {
		// Here's we're getting responses from acks
		HSPacket r = HSPacket(response);
		
		// Debugging information
		debug << "Recieved response packet from: " << r.sourceIP() << " on port: " << r.sourcePort();
		cout << colorize(debug.str(), "red");
		debug.str("");
		
		// Always enable in topology if we got a response packet.
		topology.enable(r.sourceIP(), r.sourcePort());
		
		string ackFromIP = r.sourceIP();
		int ackWithPort = r.sourcePort();
		
		stringstream ss;
		// Remove them from acksNeededFromNeighbors

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
			// cout << "No more acks needed from neighbors! " << endl;
			// We got all our acks no need to disable a node in our topology. or send in the fire squad (ie. run createroutes)
		}
	} 
	else {
		// Should be a linkstatepacket - forward it on to all neighbors EXCEPT the one we recieved it from
		// After its sent out we should start receiving ack packets from our neighbors
		LSPacket LSP = LSPacket(response);
		
		// Check to see if we should update our topology and send out LSPs and whatnot, by comparing sequence numbers
		// Also will not forward on packets when TTL reaches 0 - this will prevent a buildup of packets in the network.
		if(!shouldUpdate(LSP)) return;
		
		stringstream write;
		write << "Recieved an LSP Packet from " << LSP.id() << " with a TTL of " << LSP.TTL() << " and a seqNo of " << LSP.sequenceNumber();
		
		cout << colorize(write.str(), "pink");
		
		vector<Node> neighbors = topology.neighborsFromString(LSP.neighbors());
		
		// cout << "Disabled: " << disabled.at(0).host() << " Port: " << disabled.at(0).port() << endl;
		
		// Update the topology
		for(unsigned int i = 0; i < neighbors.size(); i++) {
			Node neighbor = neighbors.at(i);
			neighbor = topology.get(neighbor);
			if(neighbor.online()) {
				topology.enable(neighbor.host(), neighbor.port());
			} else {
				topology.disable(neighbor.host(), neighbor.port());
			}
		}
		
		// Decrement the TTL;
		int TTL = LSP.TTL();
		TTL--;
		
		// Forward LSP packet to all neighbors except the one we just received from
		string ID = LSP.id();
		map<string, string> IDInfo = fromID(ID);
		string host = IDInfo["host"];
		int port = atoi(IDInfo["port"].c_str());
		
		
		vector<Node> sendTo;
		for(unsigned int i = 0; i < me.neighbors().size(); i++) {
			Node n = me.neighbors().at(i);
			if(host.compare(n.host()) != 0 && port != n.port()) {
				sendTo.push_back(n);
			}
		}
		
		createroutes(sendTo, TTL);

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
	me = topology.get(host, port);
	
	
	Socket sock = Socket();
	sock.block(false);
	
	// Start the timer.
	int messageNeighborsWait = 4000;
	timer.add("messageNeighbors", messageNeighborsWait);
	timer.add("updateLSP", 20000);
	timer.add("decrementTTL", 1000);
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
			timer.add("messageNeighbors", messageNeighborsWait);
		}
		
		// Check to see if expiration up on acksNeededFromNeighbors
		stringstream key;
		for(unsigned int i = 0; i < acksNeededFromNeighbors.size(); i++) {
			Node n = acksNeededFromNeighbors.at(i);
			key.str("");
			key << "A|" << n.host() << ":" << n.port();
			
			if(ackTimer.isDone(key.str())) {
				// This node did not respond. Disable in topology
				topology.disable(n.host(), n.port());
				
				stringstream write;
				string neighbors = topology.getNeighborsForLSP(me);
				write << "Something has changed. Send emulators LS information: " << neighbors << " to neighbors.";
				cout << colorize(write.str(), "yellow");

				// Then trigger reliable flooding algorithm to update node topologies
				createroutes(me.neighbors(), 10);
			}
 		}
		
		if(timer.isDone("updateLSP")) {
			cout << colorize("Occasional update... flood topology", "blue");
			createroutes(me.neighbors(), 10);
			timer.add("updateLSP", 20000);
		}
		
		// if(timer.isDone("decrementTTL")) {
		// 	decrementTTL();
		// 	timer.add("decrementTTL", 10000);
		// }
		
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
