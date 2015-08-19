

#include "BinaryStreamPostTask.h"

BinaryStreamPostTask::BinaryStreamPostTask(const char *url, const char *body)
	:BinaryStreamTask(url)
	,dataBody_(body)
{

}

const char* BinaryStreamPostTask::bodyForPostData()
{
	return dataBody_.c_str();
}