
#include "PhaseMan.h"
#include "task/BinaryFileTask.h"
#include "task/PlainTextTask.h"
#include "thread/ThreadPool.h"
#include "thread/WorkloadWrapper.h"
#include "pathhelper/PathHelper.h"

#include <vector>
#include <string>
#include <functional>

#include "xxhashwrapper.h"
#include "MapInfo.h"
#include <cstdlib>

#define _MaxFailPerItem 5

void ScheduleDownload(HotTaskIn *_pHot__, TaskMan *taskMan) {
    std::string taskURL = PathHelper::formatResourceUri(_pHot__);
    std::string savePath = PathHelper::formatCachePath(_pHot__);
	auto pHotInfo = new HotTaskIn(*_pHot__);
	char* pSave = strdup(savePath.c_str());
	SimpleTask* pTask = new BinaryFileTask(taskURL.c_str(), pSave);
	Assign(WorkloadWrapper::create([=]{
		return pTask->perform();
	}, [=] {
		delete pHotInfo;
		delete pTask;
		free(pSave);
		//--------
	}, [=] {
		VerifyOneByOne(pHotInfo, taskMan);
	}, [=] {
		// If failed , schedule again. 
		pHotInfo->incFail();
		if(pHotInfo->failCount() > _MaxFailPerItem){
			taskMan->notifyTaskFinish(true);//Too many failure for download.
		} else {
			ScheduleDownload(pHotInfo, taskMan);
		}
	}));
}

void VerifyOneByOne(HotTaskIn *_pHot__, TaskMan *taskMan){
	HotTaskIn *pHotInfo = new HotTaskIn(*_pHot__);
	Assign(WorkloadWrapper::create([=]{
        std::string cache_file_path = PathHelper::formatCachePath(pHotInfo);
        std::string cache_file_abs = PathHelper::getInstance().getWritablePath() + cache_file_path;
		unsigned int hex = calculateFileXXHASH(cache_file_abs.c_str());
		char buf[32];
		snprintf(buf, sizeof(buf), "%08x", hex);
		//printf("Verifying %s to %s...\n", pHotInfo->xxHashStr(), buf);
		return strcmp(buf, pHotInfo->xxHashStr());
	}, [=]{
		delete pHotInfo;
	}, [=]{
		printf("Verifying of \"%s\" is success !\n", pHotInfo->relatePath());
		taskMan->notifyTaskFinish(false);//Notify verification success
	}, [=]{
		//~Failed, 
		ScheduleDownload(pHotInfo, taskMan);
	}));
}

void PhaseOne(const char *_versionCode__, const char *_baseServer__, const WhenFinish& done, const WhenStep &step) {
	char taskURI[BUFSIZ];
	snprintf(taskURI, sizeof(taskURI), "%s/resfolder/hashv/%s.txt", _baseServer__, _versionCode__);
	//~ And a new download is initiated. If so.
	
	PlainTextTask *ppt = new PlainTextTask(taskURI);
	char *baseSvr = strdup(_baseServer__);
	Assign(WorkloadWrapper::create([=]{
		return ppt->perform();
	},[=]{
		delete ppt;
		free(baseSvr);
	},[=]{
		auto rs = XSplit(ppt->getStr(), [](char ch){return '\r'==ch || '\n'==ch;});
		//printf("Line count is %d\n", rs.size());
		//Statistics of the current batch work
		int tc = 0;
		for(const auto &line:rs) {
			TaskItemBase item(line.c_str());
			if(item){
				++tc;
			}
		}
		if( tc > 0 ){
			TaskMan *taskMan = new TaskMan(done, step, MapInfo(ppt->getStr()));
			taskMan->setTotalTask(tc);
			for(const auto &line:rs) {
				HotTaskIn item(line.c_str(), baseSvr);
				if(item){
					VerifyOneByOne(&item, taskMan);
				}
			}
			printf("Work assigned.\n");
		}
	},[=]{
		printf("Parsing failed.\n");
	}));
}