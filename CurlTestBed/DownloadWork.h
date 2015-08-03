
#ifndef _DOWNLOAD_WORK_DEF__
#define _DOWNLOAD_WORK_DEF__

#include "thread/Workload.h"
#include "task/SimpleTask.h"

class DownloadWork:public Workload
{
public:
	DownloadWork(SimpleTask *task)
		:_task(task)
	{

	}

	~DownloadWork()
	{
		delete _task;
	}

	virtual void execute()override
	{
		_task->perform();
	}

	virtual void finish()override
	{
		printf("%s is finished.\n", _task->toStr().c_str());
		delete this;
	}

	virtual void cancel()override
	{
		printf("%s is canceled.\n", _task->toStr().c_str());
		delete this;
	}

private:
	SimpleTask *_task;
};

#endif