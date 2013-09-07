#ifndef __ALCEDO_THREAD_H__
#define __ALCEDO_THREAD_H__
#include <pthread.h>
namespace alcedo
{
class Runnable
{
	public:
		virtual ~Runnable(){}
		virtual void run() = 0;
};
class Thread
{
	public:
		Thread();
		~Thraed();
		void start();
		pthread_t getTid(){ return tid_;}
	private:
		static void* _threadEntry(void *arg);
		pthread_t tid_;
		Runnable* runnable_;	
};
}
#endif//__ALCEDO_THREAD_H__
