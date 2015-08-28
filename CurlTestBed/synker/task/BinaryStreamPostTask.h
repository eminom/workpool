

#ifndef _BINARYSTREAM_POST_TASK_DEF__
#define _BINARYSTREAM_POST_TASK_DEF__

#include "BinaryStreamTask.h"

class BinaryStreamPostTask:public BinaryStreamTask
{
public:
	BinaryStreamPostTask(const std::string &url, const std::string &body);

private:
	virtual const char *bodyForPostData()final;

private:
	std::string dataBody_;
};
#endif