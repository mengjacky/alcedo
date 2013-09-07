#include "inetaddress.h"
#include <arpa/inet.h>
#include <string>
InetAddress::InetAddress(uint16_t port)
{
	addr_.sin_family = AF_INET;
	addr_.sin_addr.s_addr = htonl(INADDR_ANY);  	
	addr_.sin_port = htons(port);
}

InetAddress::InetAddress(std::string ip, uint16_t port)
{
	inet_pton(AF_INET, ip.data(), &addr_.sin_addr);
	addr_.sin_port = htons(port);
}
InetAddress::~InetAddress()
{

}
std::string InetAddress::getIp()
{
	char ip[32]	;
	inet_ntop(AF_INET, &addr_.sin_addr, ip, sizeof(ip));
	return ip;
}
