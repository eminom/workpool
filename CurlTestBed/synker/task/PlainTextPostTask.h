

#ifndef _PLAINTEXT_POST_TASK__DEF__
#define _PLAINTEXT_POST_TASK__DEF__

#include "PlainTextTask.h"

class PlainTextPostTask:public PlainTextTask
{
public:
	PlainTextPostTask(const std::string &url, const char *body);

private:
	virtual const char *bodyForPostData()override;

private:
	std::string dataBody_;
};

#endif