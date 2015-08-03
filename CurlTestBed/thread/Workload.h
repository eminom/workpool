

#ifndef _WORKLOAD_DEF__
#define _WORKLOAD_DEF__

#include <list>
#include <atomic>

class Workload
{
	friend class ThreadPool;
public:
	Workload();
	virtual ~Workload();
	
public:// Working thread interface
	virtual void execute() = 0;

public:// Scheduler thread
	virtual void finish() = 0;
	virtual void cancel() = 0;

	void start();
	bool isCanceled()const;

private:
	std::atomic_char _isCanceling;		// Manipulated by ThreadPool(these two)
	std::atomic_char _isProcessing;
};

#endif