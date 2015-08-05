

#ifndef _PATHHELPER_DEF__
#define _PATHHELPER_DEF__

#include <cstdlib>
#include <string>

class PathHelper
{
private:
    PathHelper();
    
public:
    static PathHelper& getInstance();
    void print();
    std::string getWritablePath();
    
private:
    static PathHelper _instance;
    
private:
    PathHelper(const PathHelper&);
    void operator=(const PathHelper&);
    
};

#endif