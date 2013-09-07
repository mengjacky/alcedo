#ifndef __ALCEDO_SOCKET_H__
#define __ALCEDO_SOCKET_H__
namespace alcedo
{
class InetAddress;
class Socket
{
	public:
		Socket(int fd=-1);
		~Socket();
		int createSockFd();
		void shutDown_R();
		void shutDown_W();
		int closeSocket();
		void setLinger(bool on=true, bool linger_on=false);
		int connect(InetAddress *peer_addr);
		int accept(InetAddress *peer_addr);
		int bind(InetAddress *bind_addr);
		int listen();
		int setNoDelay(bool on);	
		void setReuseAddr(bool on);
		void setNonblock();
		int Fd(){ return sockfd_;}
		void setFd(int fd){sockfd_ = fd;}
		int send(char *bytes, int len, int *save_err);
	private:
		int sockfd_;

				
};
}
#endif
