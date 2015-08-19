
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
     

