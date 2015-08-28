

#include "SimpleTask.h"
#include "CurlWrapper.h"

SimpleTask::~SimpleTask(){
}

SimpleTask::SimpleTask()
	:lastCode_(CURLE_OK)
{

}

CURLcode SimpleTask::getLastCode()
{
	return lastCode_;
}

int SimpleTask::perform()
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

	const char *bodyStr = bodyForPostData();
	if(bodyStr && strlen(bodyStr) > 0)
	{
		//~Now we specify we want a post request.
		//:>>  http://curl.haxx.se/libcurl/c/post-callback.html
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr);
	}

	CURLcode res = curl_easy_perform(curl);

	//Finally. done.
	long statusCode = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
	onFinalized(res, statusCode);
	lastCode_ = res;
	return res;
}

void SimpleTask::onPrepare()
{
}

void SimpleTask::onFinalized(CURLcode res, long statusCode)
{
	if (res != 0)
	{
		printf("[FAILED] Task incompleted.\n");
	}
	else
	{
		if(statusCode/100 != 2)
		{
			printf("[Failed: Http response with error] -> %d\n", statusCode);
		} 
		else
		{
			printf("[OK] -> %d\n", statusCode);
		}
	}
}

long SimpleTask::timeout()
{
    //Refer to http://curl.haxx.se/libcurl/c/CURLOPT_CONNECTTIMEOUT.html
    //Default is 300(seconds)
	return 30L;
}

const char *SimpleTask::bodyForPostData()
{
	return "";
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

