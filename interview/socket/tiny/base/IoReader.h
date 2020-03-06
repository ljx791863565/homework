#ifndef __IOREADER__H__
#define __IOREADER__H__

#include <string>
#include <vector>

class IoReader
{
public:
	IoReader(int fd);
	void getLineSplitedByBlank(std::vector<std::string>& buf);
};
#endif	//__IOREADER__H__
