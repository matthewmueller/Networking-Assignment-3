#import "tracepacket.hpp"
#import "utilities.hpp"
#include <iostream>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fstream>
#include <time.h>
using namespace std;

void sendTo(string hostn, int reqport, TracePacket p)
{
        time_t seconds;
        //struct hostent *server;
        //server = gethostbyname(hostn);
        struct sockaddr_in receiver_addr;
        int sock_fd;

        //char line[15] = "Hello World!";
        string pData = p.toString();

        sock_fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
        receiver_addr.sin_family = AF_INET;
        //bcopy((char *)server->h_addr,(char *)&receiver_addr.sin_addr.s_addr,server->h_length);
        receiver_addr.sin_addr.s_addr = inet_addr(hostn.c_str());
        receiver_addr.sin_port = htons(reqport);
        char *ipadd = inet_ntoa(receiver_addr.sin_addr);
        seconds = time(NULL);
        printf("Sending to: %s on port %d at time %ld \n", ipadd,reqport, seconds);
        sendto(sock_fd, pData.c_str(), 10000, 0,(struct sockaddr*)&receiver_addr,sizeof(receiver_addr));
        close(sock_fd);
}


TracePacket listenFor(int listenport)
{
        int mySocket = socket(AF_INET, SOCK_DGRAM, 0);
        if(mySocket < 0)
        {
                printf("ERROR: cannot open socket");
                exit(1);
        }

       // Fill in the address structure
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons(listenport);
       //Struct to store the client address
        struct sockaddr_in senderAddress;
        socklen_t senderAddressLength = sizeof(senderAddress);

        if(bind(mySocket, (struct sockaddr *) &address, sizeof(address)) < 0) 
        {
                printf("ERROR: unable to bind socket");
                exit(1);
        }
        char buffer[1000];
        if(recvfrom(mySocket, buffer, sizeof(buffer), 0, (struct sockaddr *)&senderAddress, &senderAddressLength) < 0) 
        {
                printf("ERROR: Problem with recieving data");
                exit(1);
        }

        // get the request packet and file name
        string str;
        for (uint i = 0; buffer[i] != '\0'; i++)
        {
                str += buffer[i];               
        }

        // get file
        TracePacket p = TracePacket(str);
        
        char *ipadd;
        ipadd = inet_ntoa(senderAddress.sin_addr);

        close(mySocket);
        // return ipaddsendTo((string)request_info["host_addr"],receiverPort, seqCount, chunk);

        return p;
        // return payload
        //return str;
}


int main (int argc, char const *argv[])
{
	int i,port,sPort,dPort;
	string sIP,dIP;
	string sHost,dHost;
	for (i = 0; i < argc; i++)
        {
           if ((strcmp("-a", argv[i]) == 0) && (i < argc - 1))
             port = atoi(argv[i + 1]);
           if ((strcmp("-b", argv[i]) == 0) && (i < argc - 1))
             sHost = argv[i + 1];
           if ((strcmp("-c", argv[i]) == 0) && (i < argc - 1))
             sPort = atoi(argv[i + 1]);
           if ((strcmp("-d", argv[i]) == 0) && (i < argc - 1))
             dHost = argv[i + 1];
	   if ((strcmp("-e", argv[i]) == 0) && (i < argc - 1))
             dPort = atoi(argv[i + 1]);

        }
	hostent * record = gethostbyname(sHost.c_str());
	if(record == NULL)
	{
		printf("%s is unavailable\n", (sHost).c_str());
		exit(1);
	}
	in_addr * address = (in_addr * )record->h_addr;
	sIP = inet_ntoa(* address);
	hostent * record2 = gethostbyname((dHost).c_str());
	if(record2 == NULL)
	{
		printf("%s is unavailable\n", (dHost).c_str());
		exit(1);
	}
	in_addr * address2 = (in_addr * )record2->h_addr;
	dIP = inet_ntoa(* address2);
	int TTL = 0;
	printf("%s dIP, %d dPort, %s sIP, %d sPort, %d tracePort  \n",dIP.c_str(),dPort,sIP.c_str(),sPort,port);
	do
	{
		TracePacket sent = TracePacket('t',TTL,sIP,sPort,dIP,dPort);
		sendTo(dIP,dPort,sent);
		TracePacket recieved = listenFor(port);
		printf("TracePacket Recieved from: %s on port %d \n", recieved.sourceIP().c_str(), recieved.sourcePort());
		if(0 == strcmp(sent.destinationIP().c_str(),recieved.sourceIP().c_str()))
		{
			//ROUTE FOUND
			break;
		}
		TTL = TTL + 1;
	}while(1==1);


	return 0;
}
