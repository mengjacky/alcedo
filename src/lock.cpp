#include "lock.h"
#include <pthread.h>
#include "utils.h"
namespace alcedo
{
MutexLock::MutexLock()
{
	pthread_mutex_init(&lock_, NULL);	
}
MutexLock::~MutexLock()
{
	pthread_mutex_destroy(&lock_);
}
int MutexLock::lock()
{
	return pthread_mutex_lock(&lock_);
}
int MutexLock::unlock()
{
	return pthread_mutex_unlock(&lock_);
}
int MutexLock::tryLock()
{
	return pthread_mutex_trylock(&lock_);
}
void RWLock::readLock()
{
	pthread_rwlock_rdlock(&rwlock_);	
}
void RWLock::writeLock()
{
	pthread_rwlock_wrlock(&rwlock_);
}
void RWLock::unlock()
{
	pthread_rwlock_unlock(&rwlock_);
}
void Monitor::lock()
{
	pthread_mutex_lock(&mutex_);
}
void Monitor::unlock()
{
	pthread_mutex_unlock(&mutex_);
}
void Monitor::wait(uint64_t timeout)
{
	if(timeout > 0)	{
		struct timespec spec;
		time_t curtime = ultis::Time::now();
		utils::Time::parse2Spec(&spec, timeout + curtime);		
		pthread_cond_wait(&cond_, mutex_,);	
	}else{
		pthread_cond_wait(&cond_, mutex_,);	
	}

}
}

