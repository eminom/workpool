

#include "PlainTextPostTask.h"

PlainTextPostTask::PlainTextPostTask(const std::string &url, const std::string &body)
	:PlainTextTask(url)
	,dataBody_(body)
{

}

const char* PlainTextPostTask::bodyForPostData()
{
	return dataBody_.c_str();
}