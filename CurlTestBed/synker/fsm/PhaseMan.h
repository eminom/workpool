
#ifndef _PHASEMAN_DEF__
#define _PHASEMAN_DEF__

#include "HotTaskItem.h"
#include "TaskMan.h"
#include <functional>


void ScheduleDownload(HotTaskItem*, TaskMan *taskMan);
void VerifyOneByOne(HotTaskItem *, TaskMan *taskMan);
void PhaseOne(const char *versionCode, const char *baseServer, const WhenFinish&);


//SOME UTILS
#define Assign(...)	ThreadPool::getInstance()->assign(__VA_ARGS__)

#endif
