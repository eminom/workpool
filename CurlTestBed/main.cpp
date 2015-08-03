
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
#include "DownloadWork.h"
#include "deps/cJSON/cJSON.h"

#define Assign(...)	ThreadPool::getInstance()->assign(__VA_ARGS__)

#define PrintThread()\
	do{\
		std::ostringstream os;\
		os<<"In Thread<"<<std::this_thread::get_id()<<">";\
		std::cout<<os.str()<<std::endl;\
	}while(0);

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

void ScheduleDownload(const char *name, int size, const char *path, const char *baseServer)
{
	char buf[BUFSIZ<<2] = "";
	char save[BUFSIZ] = "";
	snprintf(save, sizeof(save), "tmp/%s", name);
	snprintf(buf, sizeof(buf), "%s/resfolder/res/%s", baseServer, name);
	Assign(new DownloadWork(new BinaryFileTask(buf, save)));
}

void PhaseOne(const char *versionCode, const char *baseServer) {
	char buf[BUFSIZ];
	snprintf(buf, sizeof(buf), "%s/resfolder/hashv/%s.txt", baseServer, versionCode);
	PlainTextTask *ppt = new PlainTextTask(buf);
	char *baseSvr = strdup(baseServer);
	Assign(WorkloadWrapper::create([=]{
		return ppt->perform();
	},[=]{
		delete ppt;
		free(baseSvr);
	},[=]{
		auto rs = XSplit(ppt->getStr(), [](char ch){return '\r'==ch || '\n'==ch;});
		//printf("Line count is %d\n", rs.size());
		for(const auto &line:rs) {
			char path[BUFSIZ], name[BUFSIZ];
			int size = 0;
			if( 3 == sscanf(line.c_str(), "%s%d%s", name, &size, path)){
				ScheduleDownload(name, size, path, baseSvr);
			} else {
				printf("<%s>\n", line.c_str());
			}
		}
		printf("Parsing finished.\n");
	},[=]{
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