#include "alcedo.h"
namespace alcedo
{

int Request::forward(int serverId)
{

	EventReactor *reactor = conn_->getReactor();
	Connector *connector = reactor->getConnector(serverId);
	if(connector == NULL)
	{
		return NOT_MATCH_SERVER;	
	}

	Event *evt = new Event();
	evt->set_event(SEND_BACKEND_EVENT);
	evt->setHandler(connector);
	evt->arg = this;
	reactor->registerEvent(evt);
	return 0;	
}

int Request::doResponse()
{
	conn_->sendRequest(this);
}
}
