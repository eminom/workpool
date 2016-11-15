

#ifndef _PATHHELPER_DEF__
#define _PATHHELPER_DEF__

#include <cstdlib>
#include <string>
#include <functional>

class TaskItemBase;
class HotTaskItem;

//~ Relate-path : Full-path name
typedef std::function<void(const std::string&, const std::string&)> CallbackWithFilePath;

class PathHelper
{
private:
    PathHelper();
    
public:
    static PathHelper& getInstance();
    void makeSureCachePath();
    void makeSureTargetPath();
    void print();
    static std::string getWritablePath();
    bool DeployOneFile(const char *from, const char *to);
    
public:
    static const char* getCachePath();
    static const char* getTargetPath();
    static std::string formatCachePath(TaskItemBase *pItem);
    static std::string formatTargetPath(TaskItemBase *pItem);
    static std::string formatResourceUri(HotTaskItem *pItem);
	static void iterateTargetPath(const CallbackWithFilePath&);
	static bool DeleteOneFile(const std::string &relate);

private:
    static PathHelper _instance;
    
private:
    PathHelper(const PathHelper&);
    void operator=(const PathHelper&);
    
};

#endif