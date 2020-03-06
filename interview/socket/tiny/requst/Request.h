#ifndef __REQUEST__H__
#define __REQUEST__H__

#include <string>

class Request
{
public:
	void init(int fd, std::string uri);
	void execute();
	virtual ~Request(){}

protected:
	int getFileDescriptor() const;
	const std::string& getUri() const;

private:
	int fileDescriptor;
	std::string uri;
};

#endif //__REQUEST__H__
