

#include "TaskMan.h"
#include "TaskItemBase.h"

#include <cstring>
#include <cstdlib>

#include "pathhelper/PathHelper.h"
#include "base/LogComm.h"

bool TaskMan::deploy() {
	bool completed = true;
	auto vs = XSplit(_mi.content(), [](char ch){return '\r'==ch || '\n'==ch;});
	for(const auto&line:vs){
		TaskItemBase item(line);
		if(item){
            std::string from = PathHelper::formatCachePath(&item);
            std::string to   = PathHelper::formatTargetPath(&item);
			LOGW("<%s> => <%s>\n", from.c_str(), to.c_str());
            if(!PathHelper::getInstance().DeployOneFile(from.c_str(), to.c_str())){
				completed = false;
				break;
			}
		}
	}
	return completed;
}

void TaskMan::notifyTaskFinish(bool failed) {
	if (failed) {
		_failCount++;
		LOGE("Failed for one, up to %d", _failCount);
	}
	_taskCount--;
	if (0 == _taskCount) {
		if (!_failCount) {
			doneHandle_(deploy());
		} else {
			doneHandle_(false);
		}
	} else {
		float percent = (_fullTaskCount - _taskCount) / float(_fullTaskCount);
		// In android this one shall be troublesome.
		//int taskCount = _taskCount;
		//LOGE("FullTask is %d:   Left is <%d>", _fullTaskCount, taskCount);
		stepHandle_(percent);
	}
}
