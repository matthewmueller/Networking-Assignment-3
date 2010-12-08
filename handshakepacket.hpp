#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#import "utilities.hpp"

using namespace std;

class HSPacket {
	
	public:
		
		// *** Constructors *** //
		HSPacket()
		{
			_sourceIP = "";
			_sourcePort = 0;
			_destinationIP = "";
			_destinationPort = 0;
		}
		
		HSPacket(string packet)
		{
			vector<string> contents = explode(" ", packet);

			_type = contents.at(0).c_str()[0];
			
			_sourceIP = contents.at(1).c_str();
			_sourcePort = atoi(contents.at(2).c_str());
			
			_destinationIP = contents.at(3).c_str();
			_destinationPort = atoi(contents.at(4).c_str());
			
		}
		
		HSPacket(char type, string sourceIP, int sourcePort,
			string destinationIP, int destinationPort)
		{
			_type = type;
			_sourceIP = sourceIP;
			_sourcePort = sourcePort;
			_destinationIP = destinationIP;
			_destinationPort = destinationPort;
		}
		
		// *** Getters & Setters *** //
		HSPacket& type(char type) { _type = type; return *this; }
		char type() { return _type; }
		
		HSPacket& sourceIP(string sourceIP) { _sourceIP = sourceIP; return *this; }
		string sourceIP() { return _sourceIP; }
		
		HSPacket& sourcePort(unsigned short sourcePort) { _sourcePort = sourcePort; return *this; }
		unsigned short sourcePort() { return _sourcePort; }
		
		HSPacket& destinationIP(string destinationIP) { _destinationIP = destinationIP; return *this; }
		string destinationIP() { return _destinationIP; }
		
		HSPacket& destinationPort(unsigned short destinationPort) { _destinationPort = destinationPort; return *this; }
		unsigned short destinationPort() { return _destinationPort; }
		
		// *** Public Methods *** //
		string toString() 
		{
			// char *packet;
			stringstream packet("");
			
			packet << _type << " " << _sourceIP << " " << _sourcePort << " " << _destinationIP << " " << _destinationPort;
			
			return packet.str();
		}
		
	private:
		char _type;
		string _sourceIP;
		unsigned short _sourcePort;
		string _destinationIP;
		unsigned short _destinationPort;

};
