#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>

using namespace std;

class TracePacket {
	
	public:
		
		// *** Constructors *** //
		Packet()
		{
			_type = NULL;
			_TTL = 0;
			_sourceIP = 0;
			_sourcePort = 0;
			_destinationIP = 0;
			_destinationPort = 0;
		}
		
		Packet(string packet)
		{
			vector<string> contents = this->explode(" ", packet);

			_type = contents.at(0).c_str()[0];
			
			_TTL = strtoul(contents.at(1).c_str(), NULL, 0);

			_sourceIP = strtoul(contents.at(2).c_str(), NULL, 0);
			_sourcePort = atoi(contents.at(3).c_str());
			
			_destinationIP = strtoul(contents.at(4).c_str(), NULL, 0);
			_destinationPort = atoi(contents.at(5).c_str());
			
		}
		
		Packet(char type, unsigned long TTL, unsigned long sourceIP, unsigned short sourcePort,
			unsigned long destinationIP, unsigned short destinationPort)
		{
			_type = type;
			_TTL = TTL;
			_sourceIP = sourceIP;
			_sourcePort = sourcePort;
			_destinationIP = destinationIP;
			_destinationPort = destinationPort;
		}
		
		// *** Getters & Setters *** //
		Packet& type(char type) { _type = type; return *this; }
		char type() { return _type; }
		
		Packet& TTL(unsigned long TTL) { _TTL = TTL; return *this; }
		unsigned long TTL() { return _TTL; }

		Packet& sourceIP(unsigned long sourceIP) { _sourceIP = sourceIP; return *this; }
		unsigned long sourceIP() { return _sourceIP; }
		
		Packet& sourcePort(unsigned short sourcePort) { _sourcePort = sourcePort; return *this; }
		unsigned short sourcePort() { return _sourcePort; }
		
		Packet& destinationIP(unsigned long destinationIP) { _destinationIP = destinationIP; return *this; }
		unsigned long destinationIP() { return _destinationIP; }
		
		Packet& destinationPort(unsigned short destinationPort) { _destinationPort = destinationPort; return *this; }
		unsigned short destinationPort() { return _destinationPort; }
		
		// *** Public Methods *** //
		string toString() 
		{
			// char *packet;
			stringstream packet("");
			
			packet << _type << " " << _TTL << " " << _sourceIP << " " << _sourcePort << " " << _destinationIP << " " << _destinationPort;
			
			return packet.str();
		}
		
	private:
		char _type;
		unsigned long _TTL;
		unsigned long _sourceIP;
		unsigned short _sourcePort;
		unsigned long _destinationIP;
		unsigned short _destinationPort;
		
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