
#ifndef _WORKLOAD_WRAPPER_DEF__
#define _WORKLOAD_WRAPPER_DEF__

#include "Workload.h"
#include <functional>
#include "thread/AsyncNotifier.h"

class WorkloadWrapper:public Workload
{
protected:
	typedef std::function<int()> OnExecute;
	typedef std::function<void()> OnSuccess;
	typedef std::function<void()> OnFailure;
	typedef std::function<void()> OnCleanup;

protected:
	~WorkloadWrapper()
	{
	}

protected:
	virtual void execute()override
	{
		if(!_execute()){
			AsyncNotifier::getInstance()->schedule(_onSuccess);
		} else {
			AsyncNotifier::getInstance()->schedule(_onFailure);
		}
		AsyncNotifier::getInstance()->schedule(_onCleanup);
	}

	virtual void finish()override
	{
		delete this;
	}

	virtual void cancel()override
	{
		delete this;
	}

private:
	WorkloadWrapper(const OnExecute &e, const OnCleanup &c, const OnSuccess &ok, const OnFailure &fail)
		:_execute(e), _onCleanup(c), _onSuccess(ok), _onFailure(fail)
	{

	}

public:
	static Workload* create(const OnExecute &e, const OnCleanup &cleanup, const OnSuccess &ok, const OnFailure &fail)
	{
		return new WorkloadWrapper(e, cleanup, ok, fail);
	}

private:
	OnExecute _execute;
	OnCleanup _onCleanup;
	OnSuccess _onSuccess;
	OnFailure _onFailure;
};


#endif