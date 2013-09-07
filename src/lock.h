#ifndef __ALCEDO_LOCK_H__
#define __ALCEDO_LOCK_H__
class MutexLock
{
	public:
		MutexLock();
		~MutexLock();
		int lock();
		int unlock();
		int tryLock();
	private:
		pthread_mutex_t lock_;

};
class MutexLockGuard
{
	public:
		MutexLockGuard(MutexLock &mutex):
			mutex_(mutex)
		{
			mutex_.lock();	
		}
		~MutexLockGuard()
		{
			mutex_.unlock();	
		}
	private:
		MutexLock mutex_;

};
class RWLock
{
	public:
		RWLock()
		{
			pthread_rwlock_init(&rwlock_, NULL);		
		}
		~RWLock()
		{
			pthread_rwlock_destroy(&rwlock_);	
		}
		void readLock();
		void writeLock();
		void unlock();
		void tryWriteLock();
		void tryReadLock();
	private:
		pthread_rwlock_t rwlock_;
};
class Monitor
{
	public:
		Monitor()
		{
			pthread_cond_init(&cond, NULL);	
			pthread_mutex_init(&mutex_, NULL);
		}
		~Monitor()
		{
			pthread_cond_destroy(&cond);	
			pthread_cond_destroy(&mutex_);
		}
		void lock();
		void unlock();
		void wait(uint64_t timeout);
	private:
		pthread_mutex_t mutex_;
		pthread_cond_t cond_;
};
#endif //__ALCEDO_LOCK_H__
