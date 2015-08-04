

#ifndef _TASKMAN_DEF__
#define _TASKMAN_DEF__

#include <atomic>
#include <mutex>
#include <functional>
#include "EmComm.h"
#include "HotTaskItem.h"
#include "MapInfo.h"

typedef std::function<void()> WhenFinish;
typedef const WhenFinish &WhenFinishProto;

class TaskMan
{
private:
	~TaskMan()
	{

	}

public:
	TaskMan(WhenFinishProto handle, const MapInfo &mi)
		:doneHandle_(handle)
		,_mi(mi)
	{
		_taskCount = 0;
	}

	void setTotalTask(int count) {
		_taskCount = count;
	}

	void notifyTaskFinish()	{
		_taskCount--;
		if(0==_taskCount){
			if(deploy()){
				//Deploy done.
			}
			doneHandle_();
		}
	}
	
	bool deploy();

public:
	static std::string formatCachePath(TaskItemBase *pItem){
		char name_buff[BUFSIZ];
		snprintf(name_buff, sizeof(name_buff), "tmp/%s", pItem->md5name());
		return name_buff;
	}

	static std::string formatResourceUri(HotTaskItem *pItem){
		char taskURL[BUFSIZ<<2] = "";
		snprintf(taskURL, sizeof(taskURL), "%s/resfolder/res/%s", pItem->baseServer(), pItem->md5name());
		return taskURL;
	}

	static std::string formatTargetPath(TaskItemBase *pItem){
		char targetPath[BUFSIZ] = "";
		snprintf(targetPath, sizeof(targetPath), "ressrc/%s", pItem->relatePath());
		return targetPath;
	}

private:
	const WhenFinish doneHandle_;
	std::atomic_int _taskCount;
	MapInfo _mi;
	//std::mutex _mutex;

private:
	TaskMan(const TaskMan&);
	void operator=(const TaskMan&);
};

#endif