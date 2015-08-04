
#ifndef _WORKLOAD_WRAPPER_DEF__
#define _WORKLOAD_WRAPPER_DEF__

#include "Workload.h"
#include <functional>


class WorkloadWrapper:public Workload
{
protected:
	typedef std::function<int()> OnExecute;
	typedef std::function<void()> OnSuccess;
	typedef std::function<void()> OnFailure;
	typedef std::function<void()> OnCleanup;

protected:
	~WorkloadWrapper();

protected:
	virtual void execute()override;
	virtual void finish()override;
	virtual void cancel()override;

private:
	WorkloadWrapper(const OnExecute &e, const OnCleanup &c, const OnSuccess &ok, const OnFailure &fail);

public:
	static Workload* create(const OnExecute &e, const OnCleanup &cleanup, const OnSuccess &ok, const OnFailure &fail);

private:
	OnExecute _execute;
	OnCleanup _onCleanup;
	OnSuccess _onSuccess;
	OnFailure _onFailure;
};


#endif