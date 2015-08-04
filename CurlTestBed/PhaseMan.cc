
#include "PhaseMan.h"
#include "task/BinaryFileTask.h"
#include "task/PlainTextTask.h"
#include "thread/ThreadPool.h"
#include "thread/WorkloadWrapper.h"

#include <vector>
#include <string>
#include <functional>

#include "xxhashwrapper.h"


// the FSM
std::vector<std::string> XSplit(const std::string &content, std::function<bool(char)> predicator) {
	//printf("Content is %s\n", content.c_str());
	std::vector<std::string> vs;
	const char *front = content.c_str();
	const char *tail  = front + content.size();
	const char *read  = front;
	for(;read!=tail;){
		if(predicator(*read)){
			if(read - front != 0){
				vs.push_back(std::string(front, read-front));
			}
			++read;
			front = read;
		} else {
			++read;
		}
	}
	if(read - front != 0){
		vs.push_back(std::string(front, read-front));
	}
	return vs;
}

void ScheduleDownload(HotTaskItem *_pHot__, TaskMan *taskMan) {
	std::string taskURL = taskMan->formatResourceUri(_pHot__);
	std::string savePath = taskMan->formatCachePath(_pHot__);
	auto pHotInfo = new HotTaskItem(*_pHot__);
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
		ScheduleDownload(pHotInfo, taskMan);
	}));
}

void VerifyOneByOne(HotTaskItem *_pHot__, TaskMan *taskMan){
	HotTaskItem *pHotInfo = new HotTaskItem(*_pHot__);
	Assign(WorkloadWrapper::create([=]{
		char md5name[BUFSIZ];
		snprintf(md5name, sizeof(md5name), "tmp/%s", pHotInfo->md5name());
		unsigned int hex = calculateFileXXHASH(md5name);
		char buf[32];
		snprintf(buf, sizeof(buf), "%08x", hex);
		//printf("Verifying %s to %s...\n", pHotInfo->xxhash_.c_str(), buf);
		return strcmp(buf, pHotInfo->xxhash_.c_str());
	}, [=]{
		delete pHotInfo;
	}, [=]{
		printf("Verifying of \"%s\" is success !\n", pHotInfo->path_.c_str());
		taskMan->notifyTaskFinish();
	}, [=]{
		//~Failed, 
		ScheduleDownload(pHotInfo, taskMan);
	}));
}

void PhaseOne(const char *_versionCode__, const char *_baseServer__, const WhenFinish& done) {
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
			HotTaskItem item(line.c_str(), baseSvr);
			if(item){
				++tc;
			}
		}
		if( tc > 0 ){
			TaskMan *taskMan = new TaskMan(done, MapInfo(ppt->getStr()));
			taskMan->setTotalTask(tc);
			for(const auto &line:rs) {
				HotTaskItem item(line.c_str(), baseSvr);
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