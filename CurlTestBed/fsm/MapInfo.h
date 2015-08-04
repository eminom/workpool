

#ifndef _MAP_INFO__DEF__
#define _MAP_INFO__DEF__

#include <string>
#include <functional>
#include <vector>

class MapInfo
{
public:
	MapInfo(const std::string &whole)
		:_content(whole)
	{

	}

	const std::string& content()const
	{
		return _content;
	}

private:
	std::string _content;
};

std::vector<std::string> XSplit(const std::string &content, std::function<bool(char)> predicator);

#endif