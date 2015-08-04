

#include "WorkloadWrapper.h"
#include "AsyncNotifier.h"

WorkloadWrapper::~WorkloadWrapper(){
}

WorkloadWrapper::WorkloadWrapper(const OnExecute &e, const OnCleanup &c, const OnSuccess &ok, const OnFailure &fail)
		:_execute(e), _onCleanup(c), _onSuccess(ok), _onFailure(fail)
{

}

Workload* WorkloadWrapper::create(const OnExecute &e
						   , const OnCleanup &cleanup
						   , const OnSuccess &ok
						   , const OnFailure &fail) {
	return new WorkloadWrapper(e, cleanup, ok, fail);
}

void WorkloadWrapper::finish(){	delete this;}
void WorkloadWrapper::cancel(){	delete this;}

void WorkloadWrapper::execute(){
	if(!_execute()){
		AsyncNotifier::getInstance()->schedule(_onSuccess);
	} else {
		AsyncNotifier::getInstance()->schedule(_onFailure);
	}
	AsyncNotifier::getInstance()->schedule(_onCleanup);
}
