#ifndef __LF_EPOLLER_H__
#define __LF_EPOLLER_H__
#include <vector>
#include "types.h"
#include "event.h"
class Epoller
{
	public:
		typedef std::vector<struct epoll_event> EpollEventList;

		Epoller();
		~Epoller();
		int addEvent(Event *evt);
		int removeEvent(Event *evt);
		int modEvent(Event *evt);
		int epollWait(EventList *active_events, int timeout);

	private:
		static const int kFdSize_ = 1024;	
		static const int kInitEventSize = 32;	
		int op_event(IEvent *evt, int op);
		int epoll_fd_;
		EpollEventList events_;	
		//DECL_LOG(logger);
};
#endif
