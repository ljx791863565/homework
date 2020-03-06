#ifndef TINY_ERROR__H__
#define TINY_ERROR__H__

#include <string>

struct TinyError
{
	TinyError(const std::string& errNum, 
			const std::string& shortMsg, 
			const std::string& longMsg)
			: ErrNum(errNum), ShortMsg(shortMsg), LongMsg(longMsg)
	{
		
	}

	~TinyError() throw()
	{

	}

	std::string ErrNum;
	std::string ShortMsg;
	std::string LongMsg;
};

#endif
