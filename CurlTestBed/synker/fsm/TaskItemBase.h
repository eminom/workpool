

#ifndef _TASK_ITEM_BASE__
#define _TASK_ITEM_BASE__

#include <string>

class TaskItemBase {
public:
	TaskItemBase(const std::string &line)
		:ok_(false),
		rawEntry_(line) {
		int length = 0;
		char md5name[BUFSIZ], xxhash[BUFSIZ], path[BUFSIZ];
		if( 4 == sscanf(line.c_str(), "%s%d%s%s", md5name, &length, xxhash, path)){
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
	const char* md5name()const{
		return md5name_.c_str();
	}

	const char *relatePath()const{
		return path_.c_str();
	}

	const char* xxHashStr()const{
		return xxhash_.c_str();
	}

private:
	bool ok_;
	std::string md5name_;	//~ 
	std::string path_;
	std::string rawEntry_;  //~ The raw line of the list file.
	std::string xxhash_;
	int length_;
};


#endif