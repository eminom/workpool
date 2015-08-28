

#include "BinaryStreamPostTask.h"

BinaryStreamPostTask::BinaryStreamPostTask(const std::string &url, const std::string &body)
	:BinaryStreamTask(url)
	,dataBody_(body)
{

}

const char* BinaryStreamPostTask::bodyForPostData()
{
	return dataBody_.c_str();
}