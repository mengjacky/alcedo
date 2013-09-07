#include "connection.h"

namespace alcedo
{
int Connection::process(Event *evt)
{
	if(revents_ & EPOLLHUP && !(revents_ & EPOLLIN))
	{   
		//LOG_INFO(logger, "EPOLLHUP");   
	}   
	else if(revents_ & POLLNVAL)
	{   
		//LOG_INFO(logger, "POLLNVAL");   
	}   
	else if(revents_ & ERROR_EVENT)
	{   
		handleError(); 
	}   
	else if((revents_ & READ_EVENT))
	{   
		handleRead();  
	}   
	else if(revents_ & WRITE_EVENT)
	{   
		handleWrite(); 
	}   
	return 0;
}
int Connection::handleRead()
{

}
int Connection::handleWrite()
{
}
int Connection::handleError()
{

}
int Connection::sendBuffer()
{

}
int Connection::sendRequest(Request* req)
{
	if(is_server_){
		req->wrap2Buffer(out_buffer_);	
	}else{
		req->wrap2BackBuffer(out_buffer_);	
	}
	if(!(evt->get_event() & WRITE_EVENT))
	{
		evt->enableWriting();
		reactor_->updateEvent(evt);
	}
}
int Connection::sendBytes(const char* bytes, int len)
{
	if(connState != CONNECTED)
	{
		//TODO LOG NOT CONNECTED
		return NET_NOT_CONNETED;	
	}
	int nret = 0;
	int remaining = 0;
	int save_err = 0;
	if(isRunInThread())	
	{
		nret = sock_->send(bytes, len, &save_err);
		if(nret == -1)
		{
			switch(save_err)	
			{
				case EAGAIN:
				case EINTR:
					remaining = len;
					break;		
				case ECONNRESET:
				case EBADF:
				case EFAULT:
				case EINVAL:	
				default:
					//LOG
					break;
			}
		
		}
		else{
			remaining = len - nret;	
		}
	}else{
		remaining = len;	
	}
	if(remaining)
	{
		out_buffer_.append(bytes + len - remaining, remaining);	
	}
	if(!(evt->get_event() & WRITE_EVENT))
	{
		evt_->enableWriting();
		reactor_->updateEvent(evt);
	}
}
}
