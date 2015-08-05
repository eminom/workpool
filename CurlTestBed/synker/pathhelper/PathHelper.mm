
#include "PathHelper.h"
#include "fsm/HotTaskItem.h"
#include "fsm/TaskItemBase.h"
#import <Foundation/Foundation.h>
#include "base/EmComm.h"

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
    return "cache/";
}

std::string PathHelper::formatCachePath(TaskItemBase *pItem){
    char name_buff[BUFSIZ] = "";
    snprintf(name_buff, sizeof(name_buff), "cache/%s", pItem->md5name());
    return name_buff;
}

std::string PathHelper::formatTargetPath(TaskItemBase *pItem){
    char targetPath[BUFSIZ] = "";
    snprintf(targetPath, sizeof(targetPath), "ressrc/%s", pItem->relatePath());
    return targetPath;
}

std::string PathHelper::formatResourceUri(HotTaskItem *pItem){
    char taskURL[BUFSIZ<<2] = "";
    snprintf(taskURL, sizeof(taskURL), "%s/resfolder/res/%s", pItem->baseServer(), pItem->md5name());
    return taskURL;
}
