#include "epoller.h"
#include <sys/epoll.h>
//IMPL_LOG(Epoller, logger);
Epoller::Epoller():
	epoll_fd_(::epoll_create(kFdSize_)),
	events_(kInitEventSize)
{

}
Epoller::~Epoller()
{
	::close(epoll_fd_);	
}
int Epoller::epollWait(EventList *active_events, int timeout)
{
	int nfds = ::epoll_wait(epoll_fd_, &*events_.begin(), events_.size(), timeout);
	if(nfds > 0)
	{
		for(int i = 0; i < nfds; i++)	
		{
			Event *evt = static_cast<Event*>(events_[i].data.ptr);
			evt->set_revents(events_[i].events);
			active_events->push_back(evt);
		}
	}
	else if(nfds == 0)
	{
	
	}
	else
	{
	
	}
	if(nfds == static_cast<int>(events_.size()))
	{
		events_.resize(nfds * 2);	
	}
	return nfds;
}
int Epoller::addEvent(Event *evt)
{
	return (op_event(evt, EPOLL_CTL_ADD));	
}

int Epoller::removeEvent(Event *evt)
{
	if(evt->is_none_event())			
	{
		return (op_event(evt, EPOLL_CTL_DEL));		
	}
	return -1;
}
int Epoller::modEvent(Event *evt)
{
	return (op_event(evt, EPOLL_CTL_MOD));	
}
int Epoller::opEvent(Event *evt, int op)
{
	struct epoll_event epoll_evt;
	memset(&epoll_evt, 0, sizeof(epoll_event));	
	epoll_evt.data.fd = evt->getHandle();
	epoll_evt.data.ptr = evt;
	epoll_evt.events = evt->revents();
	return (::epoll_ctl(epoll_fd_, op, evt->get_handle(), &epoll_evt));
}

