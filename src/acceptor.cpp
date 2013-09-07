#include "acceptor.h"
#include "endpoint.h"
namespace alcedo
{

void Acceptor::listen()
{
	acceptSocket_.createSockFd();		
	acceptSocket_.setReuseAddr(1);
	acceptSocket_.setNoDelay(1);
	acceptSocket_.bind(&srv_addr_);
	acceptSocket_.listen();
}
int Acceptor::process(EventDriver* driver, Event *evt)
{
	if(!(evt->revent() & READ_EVENT))
	{
		//TODO LOG	
		return -1;	
	}
	InetAddress *peer = new InetAddress();	
	int peer_fd = acceptSocket_->accpet(peer);	
	if(peer_fd == -1)
	{
		delete peer;	
		return -1;
	}
	Endpoint::getInstance()->dispatchConn(peer_fd, peer, type_, codec_);	
}
}
