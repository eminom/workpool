

#ifndef _HOTTASK_IN__DEF___
#define _HOTTASK_IN__DEF___

#include "HotTaskItem.h"

class HotTaskIn:public HotTaskItem
{
public:
	HotTaskIn(const char *line, const char *baseUrl);

public:
	int failCount()const;
	void incFail();

private:
	int failedCount_;
};


#endif