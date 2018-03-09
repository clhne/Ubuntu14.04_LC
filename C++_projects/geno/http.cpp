//
// Created by geno1024 on 17-8-17.
//

#include <string>
#include "http.h"

/**
 * Default constructor.
 */
http::http() : networking()
{
	sockfd = createTCPSocket();
}

/**
 * Default constructor, with given ip and port.
 * @param ip
 * @param port
 */
http::http(const char *ip, unsigned short port) : networking()
{
	sockfd = createTCPSocket();
	http::connect(ip, port);
}

/**
 * Set connect ip and port.
 * @param ip
 * @param port
 * @return 0 if success else -1
 */
int http::connect(const char *ip, unsigned short port)
{
	return networking::connectSocket(ip, port);
}

/**
 * Write a GET request to the socket
 * @param url
 * @return bytes written
 */
long http::get(const char *url)
{
	return networking::sendData((((std::string) "GET ") + url + ((std::string) " HTTP/1.1\r\n")).c_str());
}

/**
 * Write HTTP Header to the socket
 * @param key
 * @param value
 * @return bytes written
 */
long http::setHeader(const char *key, const char *value)
{
	return networking::sendData(((std::string) key + ": " + (std::string) value + "\r\n").c_str());
}

/**
 * Write HTTP Header "Host" to the socket
 * @param host
 * @return bytes written
 */
long http::setHost(const char *host)
{
	return http::setHeader("Host", host);
}

/**
 * Write HTTP Header "User-Agent" to the socket
 * @param userAgent
 * @return bytes written
 */
long http::setUserAgent(const char *userAgent)
{
	return http::setHeader("User-Agent", userAgent);
}

/**
 * Executes this request
 * @return response
 */
const char *http::exec()
{
	networking::sendData("\r\n\r\n");
	return http::recvData();
}

// TODO other types of header, and POST method
// TODO make headers sorted independent during use
// TODO directly using a URL