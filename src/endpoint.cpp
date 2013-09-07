#include "endpoint.h"
#include "connection.h"
#include "event.h"
namespace alcedo
{
void Endpoint::dispatchConn(int fd, 
		InetAddress *addr, 
		ConnType type, 
		ICodecProtocol *codec)
{
	//connection pool
	Event *evt = new Event();
	evt->set_event(ACCEPT_EVENT);

	Connection *conn = new Connection(fd,
							codec,
							type,
							true);	
	evt->setHandler(conn);
	//add to event driver
	EventReactor* reactor = pool_->getNextReactor();
	reactor->addEvent(evt);
}
void Endpoint::start()
{
	//listen
	for(int i = 0; i < static_cast<int>(server_infos_.size()); ++i){
		Endpoint* info = server_infos_[i];		
		
	}
	pool_ = new EventReactorPool(io_thread_num_);		
	pool_->start();
}
}
