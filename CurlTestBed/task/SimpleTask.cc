

#include "SimpleTask.h"
#include "CurlWrapper.h"

void SimpleTask::perform()
{
	onPrepare();
	CurlHandleWrapper curl;
	curl_easy_setopt(curl, CURLOPT_URL, taskUrl());
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, doWriteFunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout());  //~300 is the default
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1);	// Options from Cocos2d-X
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1 );
	CURLcode res = curl_easy_perform(curl);
	onFinalized(res);
}

void SimpleTask::onPrepare()
{
}

void SimpleTask::onFinalized(CURLcode res)
{
	if (res != 0)
	{
		printf("[FAILED]\n");
	}
	else
	{
		printf("[OK]\n");
	}
}

long SimpleTask::timeout()
{
	return 300L;
}

std::string SimpleTask::toStr()
{
	return "SimpleTask";
}


size_t SimpleTask::doWriteFunc(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	SimpleTask *pThis = static_cast<SimpleTask*>(userdata);
	return pThis->writeFunc(ptr, size, nmemb);
}

