

#ifndef _HOT_TASK_ITEM__DEF__
#define _HOT_TASK_ITEM__DEF__

#include <cstdio>
#include <string>
#include "TaskItemBase.h"

class HotTaskItem : public TaskItemBase 
{
public:
	HotTaskItem(const char *line, const char *baseUrl)
		:TaskItemBase(line)
		,baseUrl_(baseUrl){}

public:
	const char *baseServer()const{
		return baseUrl_.c_str();
	}

private:
	std::string baseUrl_;
};

#endif