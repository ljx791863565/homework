#ifndef __IOWRITER__H__
#define __IOWRITER__H__

#include <string>

class IoWriter
{
public:
	IoWriter(int fd);
	void writeString(const std::string& str);
	void writeFile(const std::string& fileNmae, int filesSize);

private:
	int fileDescriptor	;
};

#endif
