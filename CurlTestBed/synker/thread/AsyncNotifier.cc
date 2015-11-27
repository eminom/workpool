
#include "AsyncNotifier.h"

static AsyncNotifier *_instance;

AsyncNotifier::AsyncNotifier()
{

}

AsyncNotifier::~AsyncNotifier()
{

}

AsyncNotifier* AsyncNotifier::getInstance()
{
	if(!_instance)
	{
		_instance = new AsyncNotifier;
	}
	return _instance;
}

void AsyncNotifier::pump()
{
	std::lock_guard<decltype(_mutex)> lock(_mutex);
    for(auto n:_ns){n();}
	_ns.clear();
}

void AsyncNotifier::schedule(const Notice &notice)
{
	std::lock_guard<decltype(_mutex)> lock(_mutex);
	_ns.push_back(notice);
}