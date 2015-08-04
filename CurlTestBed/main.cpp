
#include <string>
#include <functional>
#include <chrono>
#include <iostream>
#include <sstream>
#include <regex>
#include <cstring>

//#define CURL_STATICLIB				// PREDEFINES
//#pragma comment(lib, "libcurl_a")		//

#include "task/PlainTextTask.h"
#include "task/BinaryFileTask.h"
#include "thread/AsyncNotifier.h"
#include "thread/ThreadPool.h"
#include "thread/Workload.h"
#include "thread/WorkloadWrapper.h"

#include "deps/cJSON/cJSON.h"

#include "DownloadWork.h"
#include "HotTaskItem.h"
#include "xxhashwrapper.h"

#define Assign(...)	ThreadPool::getInstance()->assign(__VA_ARGS__)

#define PrintThread()\
	do{\
		std::ostringstream os;\
		os<<"In Thread<"<<std::this_thread::get_id()<<">";\
		std::cout<<os.str()<<std::endl;\
	}while(0);

//Forward
void ScheduleDownload(const char *_entryRa__, const char *_baseServe__);

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

void VerifyOneByOne(HotTaskItem *_pHot__){
	HotTaskItem *pHotInfo = new HotTaskItem(*_pHot__);
	Assign(WorkloadWrapper::create([=]{
		char md5name[BUFSIZ];
		snprintf(md5name, sizeof(md5name), "tmp/%s", pHotInfo->md5name_.c_str());
		unsigned int hex = calculateFileXXHASH(md5name);
		char buf[32];
		snprintf(buf, sizeof(buf), "%08x", hex);
		//printf("Verifying %s to %s...\n", pHotInfo->xxhash_.c_str(), buf);
		return strcmp(buf, pHotInfo->xxhash_.c_str());
	}, [=]{
		delete pHotInfo;
	}, [=]{
		printf("Verifying of \"%s\" is success !\n", pHotInfo->path_.c_str());
	}, [=]{
		//~Failed, 
		ScheduleDownload(pHotInfo->rawEntry_.c_str(), pHotInfo->baseUrl_.c_str());
	}));
}

void ScheduleDownload(const char *_entryRa__, const char *_baseServe__) {
	auto pHotInfo = new HotTaskItem(_entryRa__, _baseServe__);
	if (!*pHotInfo) {
		delete pHotInfo;
		return;
	}

	//~ Cache input 
	char *entryRaw  = strdup(_entryRa__);
	char *baseServer= strdup(_baseServe__);

	char taskURL[BUFSIZ<<2] = "";
	char* pSave = (char*)malloc(BUFSIZ);
	snprintf(pSave, BUFSIZ, "tmp/%s", pHotInfo->md5name_.c_str());
	snprintf(taskURL, sizeof(taskURL), "%s/resfolder/res/%s", baseServer, pHotInfo->md5name_.c_str());
	SimpleTask* pTask = new BinaryFileTask(taskURL, pSave);
	Assign(WorkloadWrapper::create([=]{
		return pTask->perform();
	}, [=] {
		delete pHotInfo;
		delete pTask;
		free(pSave);
		//--------
		free(entryRaw);
		free(baseServer);
	}, [=] {
		VerifyOneByOne(pHotInfo);
	}, [=] {
		// AGAIN?  RESCHEDULE? UNTIL SUCCESS ?
		//~ IF IT NEVER SUCCESS ??
		ScheduleDownload(entryRaw, baseServer);
	}));
}

void PhaseOne(const char *_versionCode__, const char *_baseServer__) {
	char taskURI[BUFSIZ];
	snprintf(taskURI, sizeof(taskURI), "%s/resfolder/hashv/%s.txt", _baseServer__, _versionCode__);
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
		for(const auto &line:rs) {
			char path[BUFSIZ], name[BUFSIZ], xxhash[BUFSIZ];
			int size = 0;
			if( 4 == sscanf(line.c_str(), "%s%d%s%s", name, &size, xxhash, path)){
				ScheduleDownload(line.c_str(), baseSvr);
			} else {
				printf("<%s>\n", line.c_str());
			}
		}
		printf("Parsing finished.\n");
	},[=]{
		printf("Parsing failed.\n");
		printf("Stop updating.\n");
	}));
}

int main(){
	//PrintThread();
	PlainTextTask *ptt = new PlainTextTask("http://localhost:12000/version.php");
	Assign(WorkloadWrapper::create([=]{
		return ptt->perform();
	}, [=]{
		delete ptt;
	}, [=]{
		//PrintThread();
		cJSON *root = cJSON_Parse(ptt->getStr().c_str());
		cJSON *version = cJSON_GetObjectItem(root, "version");
		cJSON *cdn     = cJSON_GetObjectItem(root, "cdn");
		//And this is a string. (Sure. Preconditioned)
		printf("version -> %s\n", version->valuestring);
		PhaseOne(version->valuestring, cdn->valuestring);
		cJSON_Delete(root);
	}, [=]{
	}));

	ThreadPool::getInstance()->start();
	while(1){
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
		AsyncNotifier::getInstance()->pump();
	}
}


/*
int main() {
	ThreadPool::getInstance()->assign(new DownloadWork(
		new	PlainTextTask("http://testbed.hhmob.net:8083/longtext"))
	);
	ThreadPool::getInstance()->assign(new DownloadWork(
		new BinaryFileTask("http://testbed.hhmob.net:8083/audio", "seed_1.mp3"))
	);
	ThreadPool::getInstance()->assign(new DownloadWork(
		new BinaryFileTask("http://dldir1.qq.com/qqfile/qq/QQ7.5/15456/QQ7.5.exe", "QQ.exe"))
		);
	ThreadPool::getInstance()->start();
	while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		AsyncNotifier::getInstance()->pump();
	}
}
*/
