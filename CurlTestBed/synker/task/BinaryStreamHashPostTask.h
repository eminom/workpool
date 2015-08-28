

#ifndef _BINARYSTREAM_SHA1_POST_TASK_DEF__
#define _BINARYSTREAM_SHA1_POST_TASK_DEF__

#include "BinaryStreamPostTask.h"

class BinaryStreamHashPostTask:public BinaryStreamPostTask
{
public:
	BinaryStreamHashPostTask(const std::string &url, const std::string &body, const char *hash);
	const char *hash()const;

private:
	std::string strHash_;
};
#endif