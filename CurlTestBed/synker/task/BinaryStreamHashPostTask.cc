

#include "BinaryStreamHashPostTask.h"

BinaryStreamHashPostTask::BinaryStreamHashPostTask(const std::string &url, const std::string &body, const std::string &hash)
	:BinaryStreamPostTask(url, body)
	,strHash_(hash)
{

}

const char* BinaryStreamHashPostTask::hash()const
{
	return strHash_.c_str();
}