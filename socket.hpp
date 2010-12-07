#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

class Socket {
	
 public:
	//*** Constructors ***//
	Socket(int d = AF_INET, int t = SOCK_DGRAM, int p = 0) {
		// Domain: AF_INET: Address format is host and port number
		//		   AF_UNIX: Address format is UNIX pathname
		// Type:   SOCK_STREAM: Establishes a virtual circuit for stream (TCP)
		//		   SOCK_DGRAM: Establishes a datagram for communication (UDP)
		//		   SOCK_SEQPACKET: Establishes a reliable, connection based
		// Protocol: Usually 0, so that type defines the connection within domain.
		domain = d;
		type = t;
		protocol = p;

		this->initialize();
	}
	
	Socket(string t) {
		if(t.compare("UDP") == 0) {
			domain = AF_INET;
			type = SOCK_DGRAM;
			protocol = 0;
		} else if(t.compare("TCP") == 0) {
			cout << "TCP";
		} else {
			cout << "Error in initializing Socket: Please specify either UDP or TCP";
			exit(1);
		}

		this->initialize();
	}
	
	//*** Public Methods ***//
	void send(string host, int port, string data) {
		
		// Formatting the structure that will send to specified client
		struct sockaddr_in client;
		client.sin_family = domain;
		client.sin_port = htons(port);

		struct hostent *clientName;

		if((clientName = gethostbyname(host.c_str())) == NULL) {
			cout << "ERROR: Invalid server name" << endl << endl;
			exit(1);
		}

		bcopy((char *)clientName->h_addr, (char *)&client.sin_addr.s_addr, clientName->h_length);	


		const char *buffer = data.c_str();
		int dataSize = data.size();

		// Send the information
		sendto(sock, buffer, dataSize, 0, (struct sockaddr*) &client, sizeof(client));
	}
	
	string listen() {
		
		// Struct to store the client address
		struct sockaddr_in senderAddress;
		socklen_t senderAddressLength = sizeof(senderAddress);


		char buffer[1400];

		// Make sure the buffer is empty
		memset(&buffer[0], 0, sizeof(buffer));

		if(recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&senderAddress, &senderAddressLength) < 0)
		{
			if(blocking) {
				printf("ERROR: Problem with recieving data\n");
				exit(1);
			}
		}

		string str;

		for(uint i = 0; buffer[i] != '\0'; i++) {
		 str += buffer[i];
		}

		return str;
	}
	
	string listen(int port) {
		if(port != boundTo) {
			this->bind(port);
			boundTo = port;
		}

		return this->listen();
	}
	
	void disconnect() {
		close(sock);
	}
	
	void reconnect() {
		this->initialize();
	}
	
	void block(bool block = true) {
		if(!block) {
			int flags = fcntl(sock, F_GETFL);
			flags |= O_NONBLOCK;
			fcntl(sock, F_SETFL, flags);
			blocking = false;
		}	
	}
	
	void bind(int port) {
		struct sockaddr_in address;
		address.sin_family = domain;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);


		if(::bind(sock, (struct sockaddr *) &address, sizeof(address)) < 0)
		{
	        printf("ERROR: unable to bind socket");
	        exit(1);
		}	
	}
	
 private:
	int domain;
	int type;
	int protocol;
	int sock;
	int errorno;
	bool blocking;
	int boundTo; 
	
	void initialize() {
		// Initialize the Socket Structure
		sock = socket(domain, type, protocol);
		this->block();
		boundTo = -1;
	}

};

