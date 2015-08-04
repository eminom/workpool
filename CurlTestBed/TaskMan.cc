

#include "TaskMan.h"
#include "TaskItemBase.h"

#include <cstring>
#include <cstdlib>

#ifdef _MSC_VER

#include <windows.h>
#include <shlobj.h>

void DeployOneFile(const char *from, const char *to){
	const char* rh = strrchr(to, '/');
	if(rh){
		char subpath[BUFSIZ] = "";
		strncpy(subpath, to, rh - to);
		char cur[MAX_PATH+1];
		GetCurrentDirectoryA(MAX_PATH, cur);
		strcat(cur, "/");
		strcat(cur, subpath);
		SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES)};
		int res = SHCreateDirectoryExA(NULL, cur, &sa);
		//printf("Creating result = %d\n", res);
	}
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