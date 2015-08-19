

#include "BinaryStreamSha1PostTask.h"

BinaryStreamPostSha1Task::BinaryStreamPostSha1Task(const char *url, const char *body, const char *sha1)
	:BinaryStreamPostTask(url, body)
	,strSha1_(sha1)
{

}

const char* BinaryStreamPostSha1Task::sha1()const
{
	return strSha1_.c_str();
}