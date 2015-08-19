

#ifndef _BINARYSTREAM_SHA1_POST_TASK_DEF__
#define _BINARYSTREAM_SHA1_POST_TASK_DEF__

#include "BinaryStreamPostTask.h"

class BinaryStreamPostSha1Task:public BinaryStreamPostTask
{
public:
	BinaryStreamPostSha1Task(const char *url, const char *body, const char *sha1);
	const char *sha1()const;

private:
	std::string strSha1_;
};
#endif