
#include "PathHelper.h"
#include "fsm/HotTaskItem.h"
#include "fsm/TaskItemBase.h"
#import <Foundation/Foundation.h>
#import <Foundation/NSException.h>
#include "base/EmComm.h"

#define _PH_TARGET_PATH     "ressrc/"
#define _PH_CACHE_PATH      "cache/"


#define _LONG_PATH  1024

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


void walkPath(const std::string &root, const std::string &current, const CallbackWithFilePath &cb) {
    if(current.size() > 0 && current.back() != '/'){
        NSLog(@"Need to know that:current does not end with a slash");
    }
    NSString *now = [NSString stringWithUTF8String:current.c_str()];
    NSFileManager *fm = [NSFileManager defaultManager];
    NSError *err = nil;
    NSArray<NSString *> *entList = [fm contentsOfDirectoryAtPath:now error:&err];
    if(err!=nil){
        NSLog(@"Error searching path %@", now);
        return;
    }
    NSMutableArray<NSString*> * dirList = [[[NSMutableArray alloc]init]autorelease];
    for(NSString *ent:entList){
        BOOL isDir = NO;
        NSString * npath = [NSString stringWithFormat:@"%@%@",now, ent];
        BOOL bExist = [fm fileExistsAtPath:npath isDirectory:&isDir];
        if(bExist){
            if(isDir){
                [dirList addObject:ent];
            } else {
                const char *fullSpec = [npath UTF8String];
                const char *relate = fullSpec + root.size();
                while( *relate && *relate == '/')
                    ++relate;
                cb(relate, fullSpec);
            }
        } else {
            NSLog(@"File<%@> is missing ??", ent);
        }
    }
    for(NSString *dir:dirList){
        // Always ends with a slash `/'
        NSString *full = [NSString stringWithFormat:@"%@%@/", now, dir];
        const char *toDir = [full UTF8String];
        walkPath(root, toDir, cb);
    }
}

void PathHelper::iterateTargetPath(const CallbackWithFilePath &cb)
{
    char startSpec[_LONG_PATH + 100];
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

bool PathHelper::DeleteOneFile(const std::string &relate) {
    std::string pre = PathHelper::getInstance().getWritablePath();
    const char* target = getTargetPath();
    std::string full = pre + target + relate;
    NSString *dst = [NSString stringWithUTF8String:full.c_str()];
    NSError *err = nil;
    BOOL rv = [[NSFileManager defaultManager] removeItemAtPath:dst error:&err];
    if(err!=nil){
        NSLog(@"Deleting error for<%s> : %@", relate.c_str(), err);
    }
    return rv == YES;
}
