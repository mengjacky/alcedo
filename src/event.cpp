#include "event.h"
namespace alcedo
{
	
void EventReactor::loop()
{
	//handle io event
	//handle pending event
	EventList tmp_list;
	int ret;
	swapTo(tmp_list);
	for(int i = 0; i < static_cast<int>(tmp_list.size()); i++)
	{
		Event *evt = tmp_list[i];	
		if(evt->get_event() == UPDATE_EVENT)
		{
			updateEvent(evt);	
		}else{
			ret = evt->getEventHandler()->process(evt);	
		}
	}
}
void EventReactor::registerEvent(Event *evt)
{
	
	if(evt->getEvent() == ADD_EVENT)	
	{
		if(epoller_.addEvent(evt))	
		{
			//TODO LOCK
			ioEvents_.push_back(evt);
		}
	}
	else
	{
		//TODO LOCK
		pendingEvents_.push_back(evt);			
	}
}
void EventReactor::updateEvent(Event *evt)
{
	if(isRunInThread())
	{
		epoller_.modEvent(evt);	
	}
	else
	{
		pendingEvents_.push_back(evt);			
	}
}
}
