#ifndef __ALCEDO_ACCEPTOR_H__
#define __ALCEDO_ACCEPTOR_H__
#include "event.h"
#include "socket.h"
namespace alcedo
{
class Acceptor: public EventHandler
{
	public:
		Acceptor(int idx, 
				InetAddress &srv_addr, 
				ConnType type, ICodecProtocol *codec):
			idx_(idx),
			srv_addr_(srv_addr),
			accept_evt(new Event(),
			type_(type),
			codec_(codec))
		{
		}
		~Acceptor()
		{}
		virtual int process(Event *evt);
		virtual int getHandle(){ accepotSocket_.Fd()}
		void listen();
	private:
		int idx_;
		Socket acceptSocket_;
		InetAddress srv_addr_;
		ConnType type_;
		ICodecProtocol *codec_;

};
}
#endif //__ALCEDO_ACCEPTOR_H__
