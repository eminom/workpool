

#ifndef _ASYNC_TASK_NOTIFIER_DEF__
#define _ASYNC_TASK_NOTIFIER_DEF__

#include <functional>
#include <list>
#include <mutex>

class AsyncNotifier
{
public:
	typedef std::function<void()> Notice;

private:
	AsyncNotifier();
	~AsyncNotifier();

public:
    void pump();
	void schedule(const Notice& notice);
	static AsyncNotifier* getInstance();

private:
    std::list<Notice> _ns;
    std::mutex _mutex;
};


#endif