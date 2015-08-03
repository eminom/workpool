

#ifndef _PLAINTEXT_TASK__DEF__
#define _PLAINTEXT_TASK__DEF__

#include "SimpleTask.h"
#include <string>

class PlainTextTask:public SimpleTask
{
private:	
	virtual size_t writeFunc(void *ptr, size_t size, size_t nmemb)override;
	virtual const char *taskUrl()override;

public:
	PlainTextTask(const char *url);

public:
	virtual void onPrepare()override
	{
		_chunk = "";
	}

	virtual void onFinalized(CURLcode res)override
	{
		__super::onFinalized(res);
		if ( ! res)
		{
			printf("%s\n", _chunk.c_str());
		}
	}

	virtual std::string toStr()
	{
		char buf[BUFSIZ];
		sprintf(buf, "Plain-text download:%s", _taskUrl.c_str());
		return buf;
	}

private:
	std::string _chunk;
	const std::string _taskUrl;
};

#endif