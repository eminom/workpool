
#include <string>
#include <functional>
#include <chrono>
#include <iostream>

//#define CURL_STATICLIB				// PREDEFINES
//#pragma comment(lib, "libcurl_a")		//

#include "task/PlainTextTask.h"
#include "task/BinaryFileTask.h"
#include "thread/AsyncNotifier.h"
#include "thread/ThreadPool.h"
#include "thread/Workload.h"

#include "DownloadWork.h"

int main() {
	ThreadPool::getInstance()->assign(new DownloadWork(
		new	PlainTextTask("http://testbed.hhmob.net:8083/longtext"))
	);
	ThreadPool::getInstance()->assign(new DownloadWork(
		new BinaryFileTask("http://testbed.hhmob.net:8083/audio", "seed_1.mp3"))
	);
	ThreadPool::getInstance()->start();
	while(1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		AsyncNotifier::getInstance()->pump();
	}
}

