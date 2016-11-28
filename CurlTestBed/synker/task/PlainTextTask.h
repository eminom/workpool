

#ifndef _PLAINTEXT_TASK__DEF__
#define _PLAINTEXT_TASK__DEF__

#include "SimpleTask.h"
#include <string>

class PlainTextTask:public SimpleTask
{
private:	
	virtual size_t writeFunc(void *ptr, size_t size, size_t nmemb)override;

public:
	PlainTextTask(const std::string &url);

public:
	virtual const char *taskUrl()override;
	virtual void onPrepare()override;
	//virtual void onFinalized(CURLcode res)override;
	virtual std::string toStr()override;
	const std::string& getStr()const;

private:
	std::string _chunk;
	const std::string _taskUrl;
};

#endif