void sendTo(string IPin, int reqport, TracePacket p)
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
        receiver_addr.sin_addr.s_addr = inet_addr(IPin.c_str());
        receiver_addr.sin_port = htons(reqport);
        char *ipadd = inet_ntoa(receiver_addr.sin_addr);
        seconds = time(NULL);
        printf("Sending to: %s on port %d at time %ld \n", ipadd,reqport, seconds);
        sendto(sock_fd, pData.c_str(), 10000, 0,(struct sockaddr*)&receiver_addr,sizeof(receiver_addr));
        close(sock_fd);
}
