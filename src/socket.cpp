#include "socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "inetaddress.h"
#include <error.h>
namespace alcedo
{
	Socket::Socket(int fd):
		sockfd_(-1)
	{
	
	}
	
	~Socket::Socket()
	{
	
	}

	int Socket::createSockFd()
	{
		if(sockfd_ == -1 && (sockfd_ = ::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, IPPROTO_TCP)) == -1)	
		{
			return -1;	
		}
		return 0;
	}
	int Socket::connect(InetAddress *peer_addr)
	{
		sockaddr_in addr = peer_addr->getSockAddr();
		int ret = ::connect(sockfd_, (struct sockaddr*)&addr, sizeof(addr));			
		int err = errno;
		if(ret < 0)
		{
			switch(err)	
			{
				case EADDRINUSE:
					//LOG addr in use	
					break;
				case EBADF:
					//LOG bad file descriptor
					break;
				case ETIMEDOUT:
					//LOG connect timeout
					break;
				case EISCONN:
					//LOG already connect
					break;
				default:
					//LOG connect error
					break;
			}
		
		}
		return ret;
	}
	int Socket::accept(InetAddress *peer_addr)
	{
		struct sockaddr_in addr = peer_addr->getSockAddr(); 
		int new_fd = ::accept(sockfd_, (struct sockaddr*)&addr, sizeof(addr));	
		if(new_fd < 0)	
		{
			int err = errno;	
			switch(err)
			{
				case EAGAIN:

					// LOG no connections are present to be accepted
					break;	
				case EMFILE:
					//LOG the per-process limit of open file descriptors has been reached
					break;	
				case ENFILE:
					//LOG he system limit on the total number of open files has been reached
					break;
				default:
					//LOG errno
					break;	
			}
		}
		return new_fd;
	}
}
void Socket::shutDown_R()
{
	if(::shutdwon(sockfd_, SHUT_RD) == -1)
	{
		//LOG	
	}
}
void Socket::shutDown_W()
{
	if(::shutdwon(sockfd_, SHUT_WR) == -1)
	{
		//LOG	
	}

}
void Socket::closeSocket()
{
	if(::close(sockfd_) == -1)
	{
		//LOG	
	}
}
void Socket::setLinger(bool on, bool linger_on)
{
	struct linger l;	
	l.l_onoff = on;
	l.l_linger = linger_on;
	::setsockopt(sockfd_, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
}
int Socket::bind(InetAddress *bind_addr)
{
	struct sockaddr_in addr = bind_addr->getSockAddr();	
	int ret = ::bind(sockfd_, (struct sockaddr*)&addr, sizeof(addr));
	if(ret < 0)
	{
		//LOG	
	}
	return ret;
}
int Socket::listen()
{
	int ret = ::listen(sockfd_, SOMAXCONN);
	if(ret < 0 && errno == EADDRINUSE)
	{
		//LOG port in use	
	}
	return ret;	
}
void Socket::setNoDelay(bool on)
{
	int optval = on ? 1 : 0;
   ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
		   &optval, sizeof optval);
}
void Socket::setReuseAddr()
{
	int optval = on ? 1 : 0;
	::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
			&optval, sizeof optval);
}
void Socket::send(const char *bytes, int len, int *save_err)
{
	int ret = ::send(sockfd_, bytes, len, MSG_DONTWAIT);
	if(ret == -1)
		*save_err = errno;
	return ret;
}
