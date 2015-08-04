
#ifndef _BINARY_FILE_TASK__DEF__
#define _BINARY_FILE_TASK__DEF__

#include "BinaryStreamTask.h"

class BinaryFileTask:public BinaryStreamTask
{
public:
	BinaryFileTask(const char *url, const char *save, int cap=1024);

	virtual void onFinalized(CURLcode res)override;
	virtual std::string toStr()override;

private:
	std::string _save;
};

#endif