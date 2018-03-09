#include <iostream>
#include <regex>
#include "networking.h"
#include "http.h"

int main()
{
	std::cout << networking::resolveHostName("www.baidu.com") << std::endl;

	/*networking *n = new networking();
	n->createTCPSocket();
	n->connectSocket("192.168.199.101", 80);
	n->sendData("GET / HTTP/1.1\r\nHost: 192.168.199.101\r\n\r\n\r\n\r\n");
	std::cout << n->recvData() << std::endl;*/


	http *h = new http("192.168.199.101", 80);
	h->get("/user_agent.php");
	h->setHost("192.168.199.101");
	h->setUserAgent("Networking in C");
	std::cout << h->exec() << std::endl;
	return 0;
}