
#ifndef _PHASEMAN_DEF__
#define _PHASEMAN_DEF__

#include "HotTaskIn.h"
#include "HotTaskItem.h"
#include "TaskMan.h"
#include <functional>


void ScheduleDownload(HotTaskIn*, TaskMan *taskMan);
void VerifyOneByOne(HotTaskIn *, TaskMan *taskMan);
void PhaseOne(const char *versionCode, const char *baseServer, const WhenFinish&, const WhenStep&);


//SOME UTILS
#define Assign(...)	ThreadPool::getInstance()->assign(__VA_ARGS__)

#endif
