

#ifndef _WORKLOAD_DEF__
#define _WORKLOAD_DEF__

#include <list>
#include <atomic>

class Workload
{
	friend class ThreadPool;
protected:
	virtual ~Workload() = 0;

protected:
	Workload();
	
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

private:
	Workload(const Workload&);
	void operator=(const Workload&);
};

#endif