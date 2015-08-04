
#ifndef _BINARY_STREAM_TASK__DEF___
#define _BINARY_STREAM_TASK__DEF___

#include "SimpleTask.h"
#include <string>


class BinaryStreamTask:public SimpleTask
{
public:
	BinaryStreamTask(const char *url, int capacity = 1024);
	~BinaryStreamTask();

public:
	virtual size_t writeFunc(void *ptr, size_t sz, size_t nmemb)override;
	virtual const char *taskUrl()override;

private:
	void realloc(int size);
	void cleanup();

protected:
	char *begin_ptr_;
	char *end_ptr_;
	char *write_ptr_;

	const std::string _taskUrl;
};


#endif