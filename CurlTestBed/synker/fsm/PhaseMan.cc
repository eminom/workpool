
#include "PhaseMan.h"
#include "task/BinaryFileTask.h"
#include "task/PlainTextTask.h"
#include "thread/ThreadPool.h"
#include "thread/WorkloadWrapper.h"
#include "pathhelper/PathHelper.h"

#include <vector>
#include <string>
#include <functional>
#include <map>

#include "xxhashwrapper.h"
#include "MapInfo.h"
#include <cstdlib>
#include "base/LogComm.h"
#include "base/EmComm.h"
#define _MaxFailPerItem 5

void ScheduleDownload(HotTaskIn *_pHot__, TaskMan *taskMan) {
    std::string __taskURL__ = PathHelper::formatResourceUri(_pHot__);
    std::string __savePath__ = PathHelper::formatCachePath(_pHot__);
	auto pHotInfo = new HotTaskIn(*_pHot__);
	char* pSave = strdup(__savePath__.c_str());
	SimpleTask* pTask = new BinaryFileTask(__taskURL__.c_str(), pSave);
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
		LOGW("Download <%s> failed.\n", pHotInfo->relatePath());
		pHotInfo->incFail();
		if(pHotInfo->failCount() > _MaxFailPerItem){
			LOGE("Download <%s> failed. Aborted.\n", pHotInfo->relatePath());
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
		LOGW("Verifying <%s> with <%s>\n", pHotInfo->xxHashStr(), buf);
		return strcmp(buf, pHotInfo->xxHashStr());
	}, [=]{
		delete pHotInfo;
	}, [=]{
		LOGW("Verifying of \"%s\" is success !\n", pHotInfo->relatePath());
		taskMan->notifyTaskFinish(false);//Notify verification success
	}, [=]{
		//~Failed, 
		LOGE("Verification failure for <%s>\n", pHotInfo->relatePath());
		pHotInfo->incFail();
		if(pHotInfo->failCount() > _MaxFailPerItem){
			LOGE("Verification for <%s> failed. Aborted.\n", pHotInfo->relatePath());
			taskMan->notifyTaskFinish(true);	//~Verification failure:(Same as download failure)
		} else {
			ScheduleDownload(pHotInfo, taskMan);
		}
	}));
}

//~ For the main thread context:
void _ProcessWithPPT(const std::string &textContent, const char *baseSvr, const WhenFinish &done, const WhenStep &step) {
	LOGW("Parsing content of version info>>>");
	auto rs = XSplit(textContent, [](char ch) {return '\r' == ch || '\n' == ch; });
	//printf("Line count is %d\n", rs.size());
	//Statistics of the current batch work
	int tc = 0;
	LOGW("Incoming task for %d", rs.size());
	for (const auto &line : rs) {
		TaskItemBase item(line.c_str());
		if (item) {
			++tc;
		} else {
			LOGE("Failed parsing for one item");
		}
	}
	if (tc > 0) {
		TaskMan *taskMan = new TaskMan(done, step, MapInfo(textContent));
		taskMan->setTotalTask(tc);
		for (const auto &line : rs) {
			HotTaskIn item(line.c_str(), baseSvr);
			if (item) {
				VerifyOneByOne(&item, taskMan);
			}
		}
		printf("Work assigned.\n");
	}
}

//~ For the main thread context:
void _ProcessWithClean(const std::string &textContext, const char *baseSvr, const WhenFinish &done, const WhenStep &step) {
	auto rs = XSplit(textContext, [](char ch) {return '\r' == ch || '\n' == ch; });
	std::map<std::string, TaskItemBase> dc;
	for (const auto &line : rs) {
		TaskItemBase item(line.c_str());
		if (item) {
			dc.insert(std::make_pair(item.relatePath(), item));
		}
	}
	std::list<std::string> toDelete;
	PathHelper::iterateTargetPath([&dc, &toDelete](const std::string &relate, const std::string&fullpath){
		bool inThisVersion = (dc.find(relate) != dc.end());
		//printf("<%s>:<%s>\n", fullpath.c_str(), (inThisVersion?"Yes":"No"));
		if (!inThisVersion) {
			LOGW("<%s>:<%s>\n", relate.c_str(), (inThisVersion ? "Yes" : "No"));
			toDelete.push_back(relate);
		}
	});
	for (auto it = toDelete.begin(); it != toDelete.end(); ++it) {
		LOGW("Deleting %s ......", it->c_str());
		if (PathHelper::DeleteOneFile(*it)) {
			LOGW("OK\n");
		} else {
			LOGE("Failed\n");
		}
	}
	done(true);
}

typedef std::function<void(const std::string&, const char *, const WhenFinish&, const WhenStep&)> PhasePostHandler;
void PhaseOneProtoFunc(
		const PhasePostHandler& postHandler,
		const char *_versionCode__, const char *_baseServer__,
		const WhenFinish &done, const WhenStep &step) { 
	char taskURI[BUFSIZ];
	snprintf(taskURI, sizeof(taskURI), "%s/resfolder/hashv/%s.txt", _baseServer__, _versionCode__);
	//~ And a new download is initiated. If so.
	PlainTextTask *ppt = new PlainTextTask(taskURI);
	char *baseSvr = strdup(_baseServer__);
	Assign(WorkloadWrapper::create([=]{
		return ppt->perform();
	}, [=] {
		delete ppt;
		free(baseSvr);
	}, [=] {
		postHandler(ppt->getStr(), baseSvr, done, step);
	}
	,[=]{
		LOGW("Parsing failed.\n");
		done(false);
	}));
}

const PhaseHandler PhaseOne = std::bind(
	PhaseOneProtoFunc
	, _ProcessWithPPT
	, std::placeholders::_1
	, std::placeholders::_2
	, std::placeholders::_3
	, std::placeholders::_4
);

const PhaseHandler PhaseOfClean = std::bind(
	PhaseOneProtoFunc
	, _ProcessWithClean
	, std::placeholders::_1
	, std::placeholders::_2
	, std::placeholders::_3
	, std::placeholders::_4
);
