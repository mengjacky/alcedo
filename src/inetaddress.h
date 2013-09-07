#ifndef __ALCEDO_INET_ADDRESS_H__
#define __ALCEDO_INET_ADDRESS_H__
#include <netinet/in.h>
#include <string>
class InetAddress
{
	public:
		InetAddress()
		{
			memset(&addr_, 0, sizeof(addr_));	
		}
		explicit InetAddress(uint16_t port);
		InetAddress(const struct sockaddr_in addr):addr_(addr)
		{

		}
		InetAddress(std::string ip, uint16_t port);
		~InetAddress();
		void setInetAddr( struct sockaddr_in addr){addr_ = addr;}
		struct sockaddr_in& getSockAddr(){return addr_;}

		std::string getIp();
	private:
		struct sockaddr_in addr_;
};
#endif
