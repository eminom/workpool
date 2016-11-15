
#ifndef _PHASEMAN_DEF__
#define _PHASEMAN_DEF__

#include "HotTaskIn.h"
#include "HotTaskItem.h"
#include "TaskMan.h"
#include <functional>


void ScheduleDownload(HotTaskIn*, TaskMan *taskMan);
void VerifyOneByOne(HotTaskIn *, TaskMan *taskMan);


//~ versionCode, baseServer, whenFinish, whenStep
//~ run in context of main thread.
typedef std::function<void(const char*, const char*, const WhenFinish&, const WhenStep&)> PhaseHandler;
extern const PhaseHandler PhaseOne;
extern const PhaseHandler PhaseOfClean;

//SOME UTILS
#define Assign(...)	ThreadPool::getInstance()->assign(__VA_ARGS__)

#endif
