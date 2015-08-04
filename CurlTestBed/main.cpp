
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
#include "PhaseMan.h"

#define PrintThread()\
	do{\
		std::ostringstream os;\
		os<<"In Thread<"<<std::this_thread::get_id()<<">";\
		std::cout<<os.str()<<std::endl;\
	}while(0);



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
