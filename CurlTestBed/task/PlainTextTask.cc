	

#include "PlainTextTask.h"
#include "base/EmComm.h"

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


void PlainTextTask::onPrepare()
{
	_chunk = "";
}

//void PlainTextTask::onFinalized(CURLcode res)
//{
//	__super::onFinalized(res);
//	if ( ! res)
//	{
//		printf("%s\n", _chunk.c_str());
//	}
//}

std::string PlainTextTask::toStr()
{
	char buf[BUFSIZ];
	snprintf(buf, sizeof(buf), "Plain-text-from<%s>", _taskUrl.c_str());
	return buf;
}

std::string PlainTextTask::getStr()
{
	return _chunk;
}