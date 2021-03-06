

#ifndef _SIMPLE_TASK__DEF__
#define _SIMPLE_TASK__DEF__

#include "CurlWrapper.h"
#include <cstdio>
#include <string>

class SimpleTask
{
private:
	virtual size_t writeFunc(void *ptr, size_t size, size_t nmemb) = 0;
	virtual long timeout();
	virtual const char *bodyForPostData();

private:
	static size_t doWriteFunc(void *ptr, size_t size, size_t nmemb, void *userdata);

protected:
	virtual void onPrepare();
	virtual void onFinalized(CURLcode res, long statusCode);

public:
	//Static should not change over time.
	int perform();
	virtual std::string toStr();
	CURLcode getLastCode();
    virtual ~SimpleTask() = 0;
	virtual const char* taskUrl() = 0;

protected:
	SimpleTask();

private:
	CURLcode lastCode_;
};

#endif