
#ifndef _DOWNLOAD_WORK_DEF__
#define _DOWNLOAD_WORK_DEF__

#include "thread/Workload.h"

class SimpleTask;
class DownloadWork:public Workload
{
private:
	~DownloadWork();

public:
	DownloadWork(SimpleTask *task);

	virtual void execute()override;
	virtual void finish()override;
	virtual void cancel()override;

private:
	SimpleTask *const _task;
};

#endif