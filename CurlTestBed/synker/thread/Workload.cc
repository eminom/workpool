

#include "Workload.h"
#include "ThreadPool.h"


Workload::Workload()
{
	_isCanceling = 0;
	_isProcessing = 0;
}

Workload::~Workload()
{

}

void Workload::start() {
    ThreadPool::getInstance()->assign(this);
}

void Workload::cancel() {
    ThreadPool::getInstance()->cancel(this);
}

bool Workload::isCanceled()const{  return _isCanceling;}


