
#include "fsm/HotTaskItem.h"
#include "fsm/TaskItemBase.h"

#include "PathHelper.h"
#include "base/EmComm.h"
#define _PH_TARGET_PATH     "ressrc/"
#define _PH_CACHE_PATH      "cache/"


//////////////////////////////////////////
//~ Heder copied from cocos2d-x Starts
#include <string>

#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>  // For O_WXXX
#include <unistd.h>
#include "platform/android/jni/JniHelper.h" 
//#include "android/asset_manager_jni.h"
//#include "deprecated/CCString.h"
//#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
//#include "base/ccUTF8.h"
#define  LOG_TAG    "Java_org_cocos2dx_lib_Cocos2dxHelper.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"
//~ cocos2d-x copied ends.

using namespace cocos2d;

//Only for this file.(Copied from cocos2d-x)
static std::string getFileDirectoryJNI() {
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
	return getFileDirectoryJNI();
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
		if(access(buf, 0)) {
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
    if(pos!=std::string::npos){
        std::string subdir = target.substr(0, pos);
		create_path(subdir);
    }
	return copyFile(source.c_str(), target.c_str());
}
     

