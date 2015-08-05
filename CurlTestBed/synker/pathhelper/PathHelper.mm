
#include "PathHelper.h"
#import <Foundation/Foundation.h>

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