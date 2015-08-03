

#ifndef _CURL_WRAPPER_DEF__
#define _CURL_WRAPPER_DEF__

#include "curl/curl.h"


class CurlHandleWrapper
{
public:
	CurlHandleWrapper()
		:_curl(nullptr)
	{
		_curl = curl_easy_init();
	}

	~CurlHandleWrapper()
	{
		if(_curl) 
		{
			curl_easy_cleanup(_curl);
			_curl = nullptr;
		}
	}

	operator CURL*()
	{
		return _curl;
	}

private:
	CURL *_curl;
};



#endif