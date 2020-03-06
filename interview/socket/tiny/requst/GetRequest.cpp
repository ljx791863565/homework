#include "GetRequest.h"
#include "../response/Response.h"

void GetRequest::doExecute()
{
	std::string filename, cgiargs;

	bool isStatic = parseUri(filename, cgiargs);
	Response(getFileDescriptor(), filename, cgiargs, isStatic).respond();
}
