

#include "PlainTextPostTask.h"

PlainTextPostTask::PlainTextPostTask(const char *url, const char *body)
	:PlainTextTask(url)
	,dataBody_(body)
{

}

const char* PlainTextPostTask::bodyForPostData()
{
	return dataBody_.c_str();
}