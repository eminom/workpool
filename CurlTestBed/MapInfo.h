

#ifndef _MAP_INFO__DEF__
#define _MAP_INFO__DEF__

#include <string>

class MapInfo
{
public:
	MapInfo(const std::string &whole)
		:_content(whole)
	{

	}

private:
	std::string _content;
};

#endif