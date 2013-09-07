#include "connector.h"
Connector::Connector(ICodecProtocol *codec, 
		InetAddress srv_addr, 
		ConnType type):
	codec(codec_),
	srv_addr_(srv_addr),
	type_(type)
{

}
Connector::~Connector()
{

}
int Connector::process(Event *evt, EventReactor *reactor)
{
	int ret = 0;
	if(evt->get_event() & SEND_BACKEND_EVENT)
	{
		Connection* conn = _getConnection(evt);
		if(!conn)
		{
			return EVENT_PROCESS_ERROR; 	
		}
		Request *req = (Request*)evt->arg;
		conn->send(req);
	}
	return EVENT_PROCESS_SUCCESS;
}
Connection* Connector::_getConnection(Event* evt)
{
	Connection* conn = NULL;
	if(free_conns.empty())
	{
		//new one	
		Socket sock;	
		if(_doConnect(sock) == -1)return NULL;
		evt->reset();
		conn = new Connection(sock->Fd(), reactor_, evt, codec_, type_);
		evt->setHandler(conn);	
		evt->enableReading();
		evt->set_event(ADD_EVENT);
		reactor_->registerEvent(evt);
		return conn;
	}
	if(type_ == kLongConnContinuity){
		conn = free_conns_.front();		
	}else{
	
		conn = free_conns_.front();
		free_conns_.pop_front();
	}
	if(conn->connState == kDisconnected)
	{
		if(!reconnect(conn))
		{
			free_conns.push_back(conn);
		}		
	}
	evt->destroy();
	return conn;
}
int Connector::_reconnect(Connection* conn)
{
	Socket sock;
	if(_doConnect(sock) == -1){
		return -1;	
	}	
	Event *evt = new Event();
	evt->set_event(ADD_EVENT);
	conn->setEvent(evt);
	conn->setFd(sock->Fd());
	reactor_->registerEvent(evt);
	return 0;
}
int Connector::_doConnect(Socket &sock)
{
	sock->createSockFd();
	return sock.connect(srv_addr_);
}	
