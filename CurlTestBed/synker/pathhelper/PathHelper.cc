
#include "fsm/HotTaskItem.h"
#include "fsm/TaskItemBase.h"

#include "PathHelper.h"
#include "base/EmComm.h"
#define _PH_TARGET_PATH     "ressrc/"
#define _PH_CACHE_PATH      "cache/"

#include <windows.h>
#include <shlobj.h>

PathHelper PathHelper::_instance;

PathHelper::PathHelper()
{

}

PathHelper& PathHelper::getInstance()
{
	return _instance;
}

std::string PathHelper::getWritablePath()
{
	char buf[BUFSIZ+MAX_PATH+1] = "";
	GetCurrentDirectoryA(MAX_PATH, buf);
	strcat(buf, "/tmp/");
	return buf;
}

void create_path(const std::string& pre)
{
	char pathbuff[BUFSIZ+MAX_PATH+1] = "";
	strcpy(pathbuff, pre.c_str());
	for(int i=strlen(pathbuff)-1;i>=0;--i){
		if('/' == pathbuff[i]){
			pathbuff[i] = '\\';
		}
	}
	SECURITY_ATTRIBUTES sa = {sizeof(sa)};
	SHCreateDirectoryExA(NULL, pathbuff, &sa);
}

void PathHelper::makeSureCachePath()
{
    std::string pre = getWritablePath();
    pre.append(getCachePath());
	create_path(pre);
}

void PathHelper::makeSureTargetPath()
{
    std::string pre = getWritablePath();
    pre.append(getTargetPath());
	create_path(pre);
}

void PathHelper::print()
{
	printf("This is windows version.\n");
	printf("Nothing curious about.\n");
}

const char* PathHelper::getCachePath(){
    return _PH_CACHE_PATH;
}

std::string PathHelper::formatCachePath(TaskItemBase *pItem){
    char name_buff[BUFSIZ] = "";
    snprintf(name_buff, sizeof(name_buff), _PH_CACHE_PATH "%s", pItem->md5name());
    return name_buff;
}

const char* PathHelper::getTargetPath(){
    return _PH_TARGET_PATH;
}

std::string PathHelper::formatTargetPath(TaskItemBase *pItem){
    char targetPath[BUFSIZ] = "";
    snprintf(targetPath, sizeof(targetPath), _PH_TARGET_PATH "%s", pItem->relatePath());
    return targetPath;
}

std::string PathHelper::formatResourceUri(HotTaskItem *pItem){
    char taskURL[BUFSIZ<<2] = "";
    snprintf(taskURL, sizeof(taskURL), "%s/resfolder/res/%s", pItem->baseServer(), pItem->md5name());
    return taskURL;
}



void walkPath(const std::string &root, const std::string &current, const CallbackWithFilePath &cb) 
{
	WIN32_FIND_DATAA find;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char dirSpec[MAX_PATH + 100];
	strcpy(dirSpec, current.c_str());
	strcat(dirSpec, "/*");
	hFind = FindFirstFileA(dirSpec, &find);
	if (INVALID_HANDLE_VALUE == hFind) {
		return;
	}
	do {
		if (find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
			if (!strcmp(find.cFileName, ".") || !strcmp(find.cFileName, "..")) {
				//~ .. and .
			} else {
				char newStart[MAX_PATH + 100];
				strcpy(newStart, current.c_str());
				strcat(newStart, "\\");
				strcat(newStart, find.cFileName);
				walkPath(root, newStart, cb);
			}
		} else {
			//Deduce the relate path from root
			char fileSpec[MAX_PATH + 100];
			strcpy(fileSpec, current.c_str());
			strcat(fileSpec, "\\");
			strcat(fileSpec, find.cFileName);
			std::string fullSpec = fileSpec;
			int off = root.size();
			char *relate = fileSpec + off;
			while (*relate && (*relate == '/' || *relate == '\\')) {
				++relate;
			}
			for (char *p = relate; *p; ++p) {
				if ('\\' == *p) {
					*p = '/';
				}
			}
			cb(relate, fullSpec);
		}
	} while (FindNextFileA(hFind, &find));
}


void PathHelper::iterateTargetPath(const CallbackWithFilePath &cb)
{
	char startSpec[MAX_PATH + 100];
	const std::string writable = getWritablePath();
	strcpy(startSpec, writable.c_str());
	const char* start = getTargetPath();
	strcat(startSpec, start);
	walkPath(startSpec, startSpec, cb);
}


bool PathHelper::DeployOneFile(const char *from, const char *to){
    std::string pre = PathHelper::getInstance().getWritablePath();
    std::string source = pre + from;
    std::string target = pre + to;
    auto pos = target.rfind("/");
    if(pos!=std::string::npos){
        std::string subdir = target.substr(0, pos);
		create_path(subdir);
    }
	return !!CopyFileA(source.c_str(), target.c_str(), FALSE);
}
     
bool PathHelper::DeleteOneFile(const std::string &relate) {
	std::string pre = PathHelper::getInstance().getWritablePath();
	const char* target = getTargetPath();
	std::string full = pre + target + relate;
	return DeleteFileA(full.c_str());
}