
#include "DownloadWork.h"
#include "task/SimpleTask.h"
#include "base/LogComm.h"

DownloadWork::~DownloadWork(){
	delete _task;
}

DownloadWork::DownloadWork(SimpleTask *task)
	:_task(task){
}


void DownloadWork::execute()
{
	_task->perform();
}

void DownloadWork::finish()
{
	LOGW("%s is finished.\n", _task->toStr().c_str());
	delete this;
}

void DownloadWork::cancel()
{
	LOGW("%s is canceled.\n", _task->toStr().c_str());
	delete this;
}