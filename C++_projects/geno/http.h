//
// Created by geno1024 on 17-8-17.
//

#ifndef NETWORKING3_HTTP_H
#define NETWORKING3_HTTP_H


#include "networking.h"

class http : public networking
{
public:
	http();
	http(const char *ip, unsigned short port = 80);
	int connect(const char *ip, unsigned short port = 80);
	long get(const char *url);

	long setHeader(const char *key, const char *value);
	long setHost(const char *host);
	long setUserAgent(const char *userAgent);

	const char *exec();
private:
	int sockfd;
};


#endif //NETWORKING3_HTTP_H
