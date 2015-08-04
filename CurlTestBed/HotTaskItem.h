

#ifndef _HOT_TASK_ITEM__DEF__
#define _HOT_TASK_ITEM__DEF__

#include <cstdio>
#include <string>

class HotTaskItem {
public:
	HotTaskItem(const char *line, const char *baseUrl)
		:ok_(false),
		rawEntry_(line),
		baseUrl_(baseUrl) {
		int length = 0;
		char md5name[BUFSIZ], xxhash[BUFSIZ], path[BUFSIZ];
		if( 4 == sscanf(line, "%s%d%s%s", md5name, &length, xxhash, path)){
			ok_ = true;
			length_  = length;
			md5name_ = md5name;
			path_    = path;
			xxhash_  = xxhash;
		}
	}
	operator bool()const{
		return ok_;
	}
private:
	bool ok_;
public:
	std::string rawEntry_;
	std::string md5name_;
	std::string xxhash_;
	std::string path_;
	std::string baseUrl_;
	int length_;
};



#endif