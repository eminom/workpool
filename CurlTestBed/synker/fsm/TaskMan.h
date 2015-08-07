

#ifndef _TASKMAN_DEF__
#define _TASKMAN_DEF__

#include <atomic>
#include <mutex>
#include <functional>
#include "base/EmComm.h"
#include "HotTaskItem.h"
#include "MapInfo.h"

typedef std::function<bool(bool)> WhenFinish;
typedef std::function<void(float)> WhenStep;
typedef const WhenFinish &WhenFinishProto;
typedef const WhenStep &WhenStepProto;

class TaskMan
{
private:
	~TaskMan()
	{

	}

public:
	TaskMan(WhenFinishProto handle, WhenStepProto step, const MapInfo &mi)
		:doneHandle_(handle)
		,stepHandle_(step)
		,_mi(mi)
		,_fullTaskCount(1)
		,_failCount(0)
	{
		_taskCount = 0;
	}

	void setTotalTask(int count) {
		_taskCount = count;
		_fullTaskCount = count > 0 ? count:1;
	}

	void notifyTaskFinish(bool failed)	{
		if(failed){
			_failCount++;
		}
		_taskCount--;
		if(0==_taskCount){
			if(!_failCount){
				doneHandle_(deploy());
			} else {
				doneHandle_(false);
			}
		} else {
			float percent = (_fullTaskCount - _taskCount) / float(_fullTaskCount);
			stepHandle_(percent);
		}
	}
	
	bool deploy();

private:
	const WhenFinish doneHandle_;
	const WhenStep   stepHandle_;
	std::atomic_int _taskCount;
	int _fullTaskCount;
	int _failCount;
	MapInfo _mi;
	//std::mutex _mutex;

private:
	TaskMan(const TaskMan&);
	void operator=(const TaskMan&);
};

#endif