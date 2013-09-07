#ifndef __ALCEDO_CONNECTION_H__
#define __ALCEDO_CONNECTION_H__
#include "event.h"
#include "buffer.h"
namespace alcedo 
{
class ICodecProtocol;
class Connection : public EventHandler
{
	public:
		
		Connection(int fd, 
				EventReactor *reactor, 
				ICodeProtocol *codec,
				ConnType type,
				bool is_server
				);
		~Connection();
		enum ConnState{kConnected, kConnecting, kDisconnected, kDisconnecting};
		virtual int process(Event* evt);		
		virtual int getHandle();
		void setPeerAddr(InetAddress peer) { peer_addr_ = peer;}
		int sendBuffer();
		int sendBytes(const char *bytes, int len);
		int sendRequest(Request *req);
		Buffer &getOutBuffer(){ return out_buffer_;}
		ConnState connState;
		EventReactor* getReactor(){ return reactor_;}		
		void setEvent(Event *evt){ evt_ = evt;}
		void setFd(int fd){ sock_.setFd(fd);}
	private:
		int _handleRead();
		int _handelWrite();
		int _handleError();
		InetAddress peer_addr_;
		EventReactor *reactor_;
		Buffer in_buffer_;
		Buffer out_buffer_; 
		Socket sock_;
		Event *evt_;
		ICodecProtocol codec_;
		bool is_server_;
};
}
#endif//__ALCEDO_CONNECTION_H__
