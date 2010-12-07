#import "tracepacket.hpp"
#import "utilities.hpp"
#import "socket.hpp"
#include <iostream>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <time.h>
using namespace std;

int main (int argc, char const *argv[])
{
	// Was there a reason for variable i...?
	//int i;
	int port,sPort,dPort;
	string sIP,dIP;
	string sHost,dHost;
	
	port = atoi(getArg("-a", argv).c_str());
	sHost = getArg("-b", argv);
	sPort = atoi(getArg("-c", argv).c_str());
	dHost = getArg("-d", argv);
	dPort = atoi(getArg("-e", argv).c_str());
	
	Socket sock = Socket();
	int TTL = 0;
	// printf("%s dIP, %d dPort, %s sIP, %d sPort, %d tracePort  \n",dIP.c_str(),dPort,sIP.c_str(),sPort,port);
	do
	{
		TracePacket sent = TracePacket('t',TTL,sIP,sPort,dIP,dPort);
		sock.send(dIP,dPort,sent.toString());
		
		string received = sock.listen(port);
		TracePacket tracePacket = TracePacket(received);
		
		printf("TracePacket Recieved from: %s on port %d \n", tracePacket.sourceIP().c_str(), tracePacket.sourcePort());
		if(sent.destinationIP().compare(tracePacket.sourceIP()) == 0)
		{
			//ROUTE FOUND
			break;
		}
		TTL = TTL + 1;
	}while(1==1);


	return 0;
}
