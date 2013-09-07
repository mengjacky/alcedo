#ifndef __ALCEDO_CONNECTOR_H__
#define __ALCEDO_CONNECTOR_H__
#include "alcedo.h"
namespace alcedo
{
class Connector : public EventHandler
{
	public:
		Connector(EventReactor *reactor, ICodecProtocol *codec, InetAddress srv_addr, ConnType type);
		~Connector();
		virtual int process(Event *evt)	;

		virtual int getHandle(){ return 0;} 
	private:
		Connection* _getConnection(Event* evt);
		int _reconnect(Connection *conn);
		int _doConnect(Socket &sock);
		//to connect address
		InerAddress srv_addr_;
		ConnType type_;
		EventReactor *reactor_;
		ConnList free_conns_;	
		InetAdress srv_addr_;
		ICodecProtocol codec_;
};
}
#endif//__ALCEDO_CONNECTOR_H__
