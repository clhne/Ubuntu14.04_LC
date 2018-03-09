//
// Created by geno1024 on 17-8-17.
//

#ifndef NETWORKING3_NETWORKING_H
#define NETWORKING3_NETWORKING_H


class networking
{
public:
	int createTCPSocket();
	int createUDPSocket();
	static char *resolveHostName(const char *host);
	int connectSocket(const char *ip, unsigned short port);
	long sendData(const char *data);
	const char *recvData();

private:
	int sockfd;
};


#endif //NETWORKING3_NETWORKING_H
