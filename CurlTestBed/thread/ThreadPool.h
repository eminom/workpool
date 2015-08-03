
// August. 2o15
// Authored by Eminem

#ifndef _THREAD_POOL_EX_DEF__
#define _THREAD_POOL_EX_DEF__

#include <mutex>
#include <list>
#include <thread>
#include <atomic>
#include <condition_variable>

class Workload;

class ThreadPool
{
	enum {_DefaultThread = 5};
	struct ThreadUnit
	{
		std::thread thread;
		std::mutex mutex;
		Workload* assignment;
		ThreadUnit()
			:assignment(nullptr)
		{
		}
	};
private:
	ThreadPool();
	~ThreadPool();

public:
	static ThreadPool* getInstance();

	void start(int threadCount=_DefaultThread);
	void stop();

	void assign(Workload*);
	void cancel(Workload*);

private:
	void listenNdAccept(ThreadUnit*);

private:
	std::list<ThreadUnit*> _threads;
	std::list<Workload*> _assignments;
	std::mutex _tmutex;
	std::condition_variable _condiv;
	std::atomic_char _stopped;
};

#endif