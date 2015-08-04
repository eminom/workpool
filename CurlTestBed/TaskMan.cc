

#include "TaskMan.h"
#include "TaskItemBase.h"

#ifdef _MSC_VER

#include <windows.h>

void DeployOneFile(const char *from, const char *to){
	CopyFileA(from, to, FALSE);
}

#else

#endif

bool TaskMan::deploy() {
	auto vs = XSplit(_mi.content(), [](char ch){return '\r'==ch || '\n'==ch;});
	for(const auto&line:vs){
		TaskItemBase item(line);
		if(item){
			std::string from = formatCachePath(&item);
			std::string to   = formatTargetPath(&item);
			printf("<%s> => <%s>\n", from.c_str(), to.c_str());
			DeployOneFile(from.c_str(), to.c_str());
		}
	}
	return true;
}