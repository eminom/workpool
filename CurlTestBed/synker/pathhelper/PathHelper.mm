
#include "PathHelper.h"
#include "fsm/HotTaskItem.h"
#include "fsm/TaskItemBase.h"
#import <Foundation/Foundation.h>
#include "base/EmComm.h"

#define _PH_TARGET_PATH     "ressrc/"
#define _PH_CACHE_PATH      "cache/"

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
    NSString *tmp = NSTemporaryDirectory();
    std::string rv = [tmp UTF8String];
    return rv;
}

void PathHelper::makeSureCachePath()
{
    std::string pre = getWritablePath();
    pre.append(getCachePath());
    NSString *path = [NSString stringWithUTF8String:pre.c_str()];
    [[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
}

void PathHelper::makeSureTargetPath()
{
    std::string pre = getWritablePath();
    pre.append(getTargetPath());
    NSString *path = [NSString stringWithUTF8String:pre.c_str()];
    [[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
}


void PathHelper::print()
{
    NSString *home = NSHomeDirectory();
    NSLog(@"Home is %@", home);
    NSString *tmp = NSTemporaryDirectory();
    NSLog(@"Temp is %@", tmp);
    NSFileManager *defaultManager = [NSFileManager defaultManager];
    
    NSString *current = [defaultManager currentDirectoryPath];
    NSLog(@"Current is %@", current);
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
        NSString *sub = [NSString stringWithUTF8String:subdir.c_str()];
        [[NSFileManager defaultManager]createDirectoryAtPath:sub withIntermediateDirectories:YES attributes:nil error:nil];
    }
    
    NSString *src = [NSString stringWithUTF8String:source.c_str()];
    NSString *dst = [NSString stringWithUTF8String:target.c_str()];
    
    NSFileManager *f_man = [NSFileManager defaultManager];
    if( [f_man contentsEqualAtPath:src andPath:dst] ){
        //NSLog(@"Equal, skip, deploy already done.");
        return true;
    }
    if( [f_man fileExistsAtPath:dst] ){
        //Need to remove the old one first.
        [f_man removeItemAtPath:dst error:nil];
    }
    
    NSError *error = nil;
    if( ! [f_man copyItemAtPath:src toPath:dst error:&error] ){
        NSLog(@"Copying %@ failed due to %@", dst, error);
		return false;//Copying failed.
    }
	return true;
}


