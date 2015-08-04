

#include "MapInfo.h"

// the FSM
std::vector<std::string> XSplit(const std::string &content, std::function<bool(char)> predicator) {
	//printf("Content is %s\n", content.c_str());
	std::vector<std::string> vs;
	const char *front = content.c_str();
	const char *tail  = front + content.size();
	const char *read  = front;
	for(;read!=tail;){
		if(predicator(*read)){
			if(read - front != 0){
				vs.push_back(std::string(front, read-front));
			}
			++read;
			front = read;
		} else {
			++read;
		}
	}
	if(read - front != 0){
		vs.push_back(std::string(front, read-front));
	}
	return vs;
}
