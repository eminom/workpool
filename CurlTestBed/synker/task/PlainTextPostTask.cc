

#include "PlainTextPostTask.h"

PlainTextPostTask::PlainTextPostTask(const std::string &url, const char *body)
	:PlainTextTask(url)
	,dataBody_(body)
{

}

const char* PlainTextPostTask::bodyForPostData()
{
	return dataBody_.c_str();
}