

#ifndef _PATHHELPER_DEF__
#define _PATHHELPER_DEF__

#include <cstdlib>
#include <string>

class TaskItemBase;
class HotTaskItem;

class PathHelper
{
private:
    PathHelper();
    
public:
    static PathHelper& getInstance();
    void makeSureCachePath();
    void makeSureTargetPath();
    void print();
    std::string getWritablePath();
    void DeployOneFile(const char *from, const char *to);
    
public:
    static const char* getCachePath();
    static const char* getTargetPath();
    static std::string formatCachePath(TaskItemBase *pItem);
    static std::string formatTargetPath(TaskItemBase *pItem);
    static std::string formatResourceUri(HotTaskItem *pItem);
    
private:
    static PathHelper _instance;
    
private:
    PathHelper(const PathHelper&);
    void operator=(const PathHelper&);
    
};

#endif