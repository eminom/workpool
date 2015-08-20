

#ifndef _PLAINTEXT_TASK__DEF__
#define _PLAINTEXT_TASK__DEF__

#include "SimpleTask.h"
#include <string>

class PlainTextTask:public SimpleTask
{
private:	
	virtual size_t writeFunc(void *ptr, size_t size, size_t nmemb)override;

public:
	PlainTextTask(const char *url);

public:
	virtual const char *taskUrl()override;
	virtual void onPrepare()override;
	//virtual void onFinalized(CURLcode res)override;
	virtual std::string toStr();
	const std::string& getStr()const;

private:
	std::string _chunk;
	const std::string _taskUrl;
};

#endif