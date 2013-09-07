#include "thread.h"
namespace alcedo
{
Thread::Thread(Runnable *runnable):
	tid(0),
	runnable_(runnable)
{
		
}
void Thread::start()
{
	int ret = pthread_create(&tid_, NULL, _threadEntry, runnable_);
	if(ret)
	{
		//LOG fail	
	}
}
void* Thread::_threadEntry(void* arg)
{
	Runnable r = static_cast<Runnable*>(arg);
	if(r)
	{
		r->run();	
	}
}
}
