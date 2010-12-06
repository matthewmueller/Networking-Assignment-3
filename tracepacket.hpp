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
		TracePacket()
		{
			_type = NULL;
			_TTL = 0;
			_sourceIP = "";
			_sourcePort = 0;
			_destinationIP = "";
			_destinationPort = 0;
		}
		
		TracePacket(string packet)
		{
			vector<string> contents = this->explode(" ", packet);

			_type = contents.at(0).c_str()[0];
			
			_TTL = strtoul(contents.at(1).c_str(), NULL, 0);

			_sourceIP = contents.at(2).c_str();
			_sourcePort = atoi(contents.at(3).c_str());
			
			_destinationIP = contents.at(4).c_str();
			_destinationPort = atoi(contents.at(5).c_str());
			
		}
		
		TracePacket(char type, int TTL, string sourceIP, int sourcePort,
			string destinationIP, int destinationPort)
		{
			_type = type;
			_TTL = TTL;
			_sourceIP = sourceIP;
			_sourcePort = sourcePort;
			_destinationIP = destinationIP;
			_destinationPort = destinationPort;
		}
		
		// *** Getters & Setters *** //
		TracePacket& type(char type) { _type = type; return *this; }
		char type() { return _type; }
		
		TracePacket& TTL(unsigned long TTL) { _TTL = TTL; return *this; }
		unsigned long TTL() { return _TTL; }

		TracePacket& sourceIP(string sourceIP) { _sourceIP = sourceIP; return *this; }
		string sourceIP() { return _sourceIP; }
		
		TracePacket& sourcePort(unsigned short sourcePort) { _sourcePort = sourcePort; return *this; }
		unsigned short sourcePort() { return _sourcePort; }
		
		TracePacket& destinationIP(string destinationIP) { _destinationIP = destinationIP; return *this; }
		string destinationIP() { return _destinationIP; }
		
		TracePacket& destinationPort(unsigned short destinationPort) { _destinationPort = destinationPort; return *this; }
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
		string _sourceIP;
		unsigned short _sourcePort;
		string _destinationIP;
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
