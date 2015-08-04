

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

public:
	const char *baseServer()const{
		return baseUrl_.c_str();
	}

	const char* md5name()const{
		return md5name_.c_str();
	}

private:
	bool ok_;
	std::string md5name_;	//~ 
	std::string baseUrl_;

public:
	std::string rawEntry_;  //~ The raw line of the list file.
	//Sub items of all.
	
	std::string xxhash_;
	std::string path_;
	int length_;
};



#endif