//
// Created by geno1024 on 17-8-17.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <string>
#include "networking.h"

/**
 * Create a TCP socket.
 * @return the socket file descriptor
 */
int networking::createTCPSocket()
{
	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	return sockfd;
}

/**
 * Create a UDP socket.
 * @return the socket file descriptor
 */
int networking::createUDPSocket()
{
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	return sockfd;
}

/**
 * Resolve host to ip address.
 * @param host
 * @return ip to the host
 */
char *networking::resolveHostName(const char *host)
{
	static char ip[16];
	struct hostent *he;
	struct in_addr **addr_list;
	if ((he = gethostbyname(host)) == nullptr)
		return nullptr;
	addr_list = (struct in_addr **) he->h_addr_list;
	for (int i = 0; addr_list[i] != nullptr; i++)
		strcpy(ip, inet_ntoa(*addr_list[i]));
	return ip;
}

/**
 * Connect the socket to ip:port.
 * @param ip
 * @param port
 * @return 0 if success else -1
 */
int networking::connectSocket(const char *ip, unsigned short port)
{
	struct sockaddr_in connection{};
	connection.sin_addr.s_addr = inet_addr(ip);
	connection.sin_family = AF_INET;
	connection.sin_port = htons(port);
	return connect(sockfd, (struct sockaddr *) &connection, sizeof(connection));
}

/**
 * Send data to the socket.
 * @param data
 * @return bytes sent
 */
long networking::sendData(const char *data)
{
	return send(sockfd, data, strlen(data), 0);
}

/**
 * Receive data from the socket.
 * @return the data received
 */
const char *networking::recvData()
{
	std::string j;
	ssize_t i;
	char reply[1024];
	while ((i = recv(sockfd, reply, 1024, 0)) > 0)
		j += ((std::string) reply).substr(0, (unsigned long) i);
	return j.c_str();
}


