
#include "HotTaskIn.h"

HotTaskIn::HotTaskIn(const char *line, const char *baseUrl)
	:HotTaskItem(line, baseUrl)
	,failedCount_(0)
{

}

void HotTaskIn::incFail()
{
	failedCount_++;
}

int HotTaskIn::failCount()const
{
	return failedCount_;
}


