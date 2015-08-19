

#ifndef _BINARYSTREAM_SHA1_POST_TASK_DEF__
#define _BINARYSTREAM_SHA1_POST_TASK_DEF__

#include "BinaryStreamPostTask.h"

class BinaryStreamHashPostTask:public BinaryStreamPostTask
{
public:
	BinaryStreamHashPostTask(const char *url, const char *body, const char *sha1);
	const char *hash()const;

private:
	std::string strHash_;
};
#endif