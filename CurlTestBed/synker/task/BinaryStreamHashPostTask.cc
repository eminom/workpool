

#include "BinaryStreamHashPostTask.h"

BinaryStreamHashPostTask::BinaryStreamHashPostTask(const char *url, const char *body, const char *hash)
	:BinaryStreamPostTask(url, body)
	,strHash_(hash)
{

}

const char* BinaryStreamHashPostTask::hash()const
{
	return strHash_.c_str();
}