#ifndef __ALCEDO_EVENT_H__
#define __ALCEDO_EVENT_H__
namespace alcedo
{
#define NONE_EVENT 0
#define READ_EVENT (POLLIN | POLLPRI)	
#define WRITE_EVENT (POLLOUT)
#define ERROR_EVENT (EPOLLERR)
#define ACCEPT_EVENT 0x16
#define UPDATE_EVENT 0x32
#define SEND_BACKEND 0x64
#define ARG_BYTE_TYPE 1
class EventReactor;
class Event
{
	public:
		Event():
			revents_(0),
			event_(0),
			arg(NULL)
		{}
		~Event()
		{}
		EventHandler *getHandler(){return handler_;}
		void setHandler(EventHandler *handler){handler_ = handler;}
		int getHandle(){ return handler->getHandle();}

		int getEvent(){return event_;}
		void set_revent(int revent){revent_ = revent;}
		void set_event(int event){ event_ = event;}
		int revent(){ return revents_;}
		
		void disableAll(){ event = NONE_EVENT;}
		void enableReading(){event |= READ_EVENT ;}
		void enableWriting(){ event |= WRITE_EVENT;}
		void disableWriting(){ event ~= WRITE_EVENT;}
		void reset(){
			revents_ = 0;
			event_ = 0;
		}
		virtual void destroyEvent();
		void destroy()
		{
			delete this;	
		}
		void* arg;
	private:
		EventHandler *handler_;
		//read from epoll
		int revents_;
		//update to epoll
		int event_;
};
class EventHandler
{
	public:
		virtual int process(Event *evt) = 0;
		virtual int getHandle() = 0;

};
class Request;
class EventReactor
{
	public:
		typedef std::vector<Event*> EventList;
		typedef std::map<int, Connector*> ConnectorMap;
		EventReactor();
		~EventReactor();
		void loop();
		void registerEvent(Event *evt);
		void updateEvent(Event *evt);
		void swapTo(EventList &event_list);
		bool isRunInThread();
		void setConnector(int serverId, Connector *connector){ connectors_[serverId] = connector;}
		Connector* getConnector(int serverId){
			ConnectorMap::iterator it = connectors_.find(serverId);		
			if(it != connectors_.end()){
				return it->second;	
			}
			return NULL;
		}
	private:
		void checkTimeout();
		Epoller epoller_;
		EventList pendingEvents_;
		EventList ioEvents_;
		ConnectorMap connctors_;
			
};
class EventReactorPool
{
	public:
		EventReactorPool(int num);
		typedef std::vector<EventReactor*> ReactorList;
		void setConnector(int serverId, Connector *connector)
		{ 
			for(int i = 0; i < static_cast<int>(reactors_.size()); i++)	
			{
				reactors_[i]->setConnector(serverId, connector);	
			}
		}
		void start();
		EventReactor* getNextReactor();
	private:
		ReactorList reactors_;
		int next_;

};
}
#endif
