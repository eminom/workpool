
#include "fsm/HotTaskItem.h"
#include "fsm/TaskItemBase.h"

#include "PathHelper.h"
#include "base/EmComm.h"

#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define _PH_TARGET_PATH     "ressrc/"
#define _PH_CACHE_PATH      "cache/"

#define _LONG_MAX	1024

//////////////////////////////////////////
//~ Heder copied from cocos2d-x Starts
#include <string>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "platform/android/jni/JniHelper.h" 
#define  LOG_TAG    "PathHelper"
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN, LOG_TAG,__VA_ARGS__)
#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"
//~ cocos2d-x copied ends.
using namespace cocos2d;

//Only for this file.(Copied from cocos2d-x)
static std::string _getFileDirectoryJNI() {
    JniMethodInfo t;
    std::string ret("");

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCocos2dxWritablePath", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }
    
    return ret;
}

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
	//~ Delegate to cocos2d-X
	std::string rv = _getFileDirectoryJNI();
	if(rv.size() > 0 && rv[rv.size()-1] != '/')
	{
		rv += "/";
	}
	return rv;
}

void create_path(const std::string& target)
{
	if(!target.size())
	{
		return;
	}
	std::string rbuf = target;
	size_t len = rbuf.size();
	if (rbuf[len-1]!='/')
	{
		rbuf += "/";
	}
	const char *pv = rbuf.c_str();
	const char *nx = strchr(pv+1, '/');
	while(nx)
	{
		char buf[BUFSIZ];
		bzero(buf, sizeof(buf));
		strncpy(buf, pv, nx - pv);
		//printf("<%s>\n", buf);
		nx = strchr(nx+1, '/');
		mkdir(buf, 0777);
		if(access(buf, 0)) 
		{
			printf("Failed to create chain <%s>\n", target.c_str());
			break;
		}
	}
	//printf("<%s>\n", str);
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
	//printf("This is windows version.\n");
	//printf("Nothing curious about.\n");
	//LOGW("This is Android version for PathHelper");
	//LOGW("Nothing new today.");
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


static
bool copyFile(const char *from, const char *to)
{
	int ifd = open(from, O_RDONLY);
	if(ifd<0)
	{
		return false;
	}
	bool rv = true;
	int openFlags = O_CREAT|O_WRONLY|O_TRUNC;
	mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	int ofd = open(to, openFlags, filePerms);
	if(ofd<0)
	{
		close(ifd);
		return false;
	}
	char buf[BUFSIZ*4];
	ssize_t readC;
	while( (readC = read(ifd, buf, sizeof(buf))) > 0 )
	{
		ssize_t writeC = write(ofd, buf, readC); //~ Dont care how many bytes are written.
		if(writeC!=readC)
		{
			LOGW("write failed. Site A100");
			rv = false;
			break;
		}
	}
	close(ifd);
	close(ofd);
	return rv;
}

bool PathHelper::DeployOneFile(const char *from, const char *to){
    std::string pre = PathHelper::getInstance().getWritablePath();
    std::string source = pre + from;
    std::string target = pre + to;
    auto pos = target.rfind("/");
    if(pos!=std::string::npos)
	{
        std::string subdir = target.substr(0, pos);
		create_path(subdir);
    }
	return copyFile(source.c_str(), target.c_str());
}

void walkPath(const std::string &root, const std::string &current, const CallbackWithFilePath &cb){
	if(current.size() > 0 && current.back() != '/'){
		printf("Oh my god\n");
		//return;  // let the error happen
	}
	struct dirent *ent = nullptr;
	DIR *pDir = opendir(current.c_str());
	char buffer[_LONG_MAX+100];
	while ((ent = readdir(pDir))!=NULL){
		if(ent->d_type&DT_DIR){
			if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0 ){
					continue;
			}
			sprintf(buffer, "%s%s/", current.c_str(), ent->d_name);
			walkPath(root, buffer, cb);
		} else {
			sprintf(buffer, "%s%s", current.c_str(), ent->d_name);
			const char *relate = buffer + root.size();
			cb(relate, buffer);
		}
	}
	closedir(pDir);
}

void PathHelper::iterateTargetPath(const CallbackWithFilePath &cb){
	std::string startSpec = getWritablePath();
	startSpec += getTargetPath();
	walkPath(startSpec, startSpec, cb);
}

bool PathHelper::DeleteOneFile(const std::string &relate){
	std::string pre = PathHelper::getInstance().getWritablePath();
	const char *target = getTargetPath();
	std::string full = pre + target + relate;
	return 0 == unlink(full.c_str());
}
     

