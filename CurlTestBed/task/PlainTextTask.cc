	

#include "PlainTextTask.h"

PlainTextTask::PlainTextTask(const char *url)
	:_taskUrl(url)
{

}

const char* PlainTextTask::taskUrl()
{
	return _taskUrl.c_str();
}


size_t PlainTextTask::writeFunc(void *ptr, size_t size, size_t nmemb)
{
	_chunk.append((char*)ptr, size * nmemb);
	return (size * nmemb);
}

