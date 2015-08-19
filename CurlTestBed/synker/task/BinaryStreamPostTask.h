

#ifndef _BINARYSTREAM_POST_TASK_DEF__
#define _BINARYSTREAM_POST_TASK_DEF__

#include "BinaryStreamTask.h"

class BinaryStreamPostTask:public BinaryStreamTask
{
public:
	BinaryStreamPostTask(const char *url, const char *body);

private:
	virtual const char *bodyForPostData()override;

private:
	std::string dataBody_;
};
#endif