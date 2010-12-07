#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#import "utilities.hpp"

using namespace std;

class LSPacket {
	
	public:
		
		// *** Constructors *** //
		LSPacket()
		{
			_id = "";
			_sequenceNumber = -1;
			_lifetime = -1;
		}
		
		LSPacket(string packet)
		{
			vector<string> contents = explode(" ", packet);
		
			_id = contents.at(0);
			// _neighbors = this->neighborsFromString(contents.at(1));
			_sequenceNumber = atoi(contents.at(2).c_str());
			_lifetime = atoi(contents.at(3).c_str());
		}
		
		LSPacket(string id, string neighbors, int sequenceNumber, int lifetime)
		{
			_id = id;
			_neighbors = neighbors;
			_sequenceNumber = sequenceNumber;
			_lifetime = lifetime;
		}
		
		// *** Getters & Setters *** //
		LSPacket& id(string id) { _id = id; return *this; }
		string id() { return _id; }
		
		LSPacket& neighbors(string neighbors) { _neighbors = neighbors; return *this; }
		string neighbors() { return _neighbors; }
		
		LSPacket& sequenceNumber(int sequenceNumber) { _sequenceNumber = sequenceNumber; return *this; }
		int sequenceNumber() { return _sequenceNumber; }
		
		LSPacket& lifetime(int lifetime) { _lifetime = lifetime; return *this; }
		int lifetime() { return _lifetime; }
		
		// *** Public Methods *** //
		string toString() 
		{
			// char *packet;
			stringstream packet("");
			
			// string neighbors = this->neighborsToString();
			
			packet << _id << " " << _neighbors << " " << _sequenceNumber << " " << _lifetime;
			
			return packet.str();
		}
		
	private:
		string _id;
		string _neighbors;
		int _sequenceNumber;
		int _lifetime;

};
