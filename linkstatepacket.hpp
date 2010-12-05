#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#import "Node.hpp"

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
			vector<string> contents = this->explode(" ", packet);
		
			_id = contents.at(0);
			_neighbors = this->neighborsFromString(contents.at(1));
			_sequenceNumber = atoi(contents.at(2).c_str());
			_lifetime = atoi(contents.at(3).c_str());
		}
		
		LSPacket(string id, vector<Node> neighbors, int sequenceNumber, int lifetime)
		{
			_id = id;
			_neighbors = neighbors;
			_sequenceNumber = sequenceNumber;
			_lifetime = lifetime;
		}
		
		// *** Getters & Setters *** //
		LSPacket& id(string id) { _id = id; return *this; }
		string id() { return _id; }
		
		LSPacket& neighbors(vector<Node> neighbors) { _neighbors = neighbors; return *this; }
		vector<Node> neighbors() { return _neighbors; }
		
		LSPacket& sequenceNumber(int sequenceNumber) { _sequenceNumber = sequenceNumber; return *this; }
		int sequenceNumber() { return _sequenceNumber; }
		
		LSPacket& lifetime(int lifetime) { _lifetime = lifetime; return *this; }
		int lifetime() { return _lifetime; }
		
		// *** Public Methods *** //
		string toString() 
		{
			// char *packet;
			stringstream packet("");
			
			string neighbors = this->neighborsToString();
			
			packet << _id << " " << neighbors << " " << _sequenceNumber << " " << _lifetime;
			
			return packet.str();
		}
		
	private:
		string _id;
		vector<Node> _neighbors;
		int _sequenceNumber;
		int _lifetime;
		
		string neighborsToString() {
			stringstream output;
			for(unsigned int i = 0; i < _neighbors.size(); i++) {
				Node n = _neighbors.at(i);
				output << n.host() << ":" << n.port() << ":" << n.online();
				if(i+1 == _neighbors.size()) break;
				output << ",";
			}
			
			return output.str();
		}
		
		vector<Node> neighborsFromString(string neighbors) {
			vector<Node> output;
			
			vector<string> nodes = explode(",", neighbors);
			for(unsigned int i = 0; i < nodes.size(); i++) {
				string node = nodes.at(i);
				vector<string>pieces = explode(":", node);
				Node n = Node(pieces[0], atoi(pieces[1].c_str()), atoi(pieces[2].c_str()));
				output.push_back(n);
			}
			
			return output;
		}
		
		vector<string> explode( const string &delimiter, const string &str)
		{
		    vector<string> arr;

		    int strleng = str.length();
		    int delleng = delimiter.length();
		    if (delleng==0)
		        return arr;//no change

		    int i=0; 
		    int k=0;
		    while( i<strleng )
		    {
		        int j=0;
		        while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
		            j++;
		        if (j==delleng)//found delimiter
		        {
		            arr.push_back(  str.substr(k, i-k) );
		            i+=delleng;
		            k=i;
		        }
		        else
		        {
		            i++;
		        }
		    }
		    arr.push_back(  str.substr(k, i-k) );
		    return arr;
		}
};
