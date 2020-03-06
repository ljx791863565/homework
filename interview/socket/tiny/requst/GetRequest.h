#ifndef __GET_REQUEST__H__
#define __GET_REQUEST__H__

#include "Request.h"
#include <string>

class GetRequest : public Request
{
	virtual void doExecute();
	bool parseUri(std::string& filename, std::stirng& cgiargs);
	bool parseStaticContentUri(std::string& filename);
	bool parseDynamicConContentUri(std::string& filename, std::string& cgiargs);
	void assignCigArgs(std::string& cgiargs);
	void doAssignCigArgs(std::string&::size_type pos, std::string& cgiargs);
};

#endif	//__GET_REQUEST__H__

