#include <stdio.h>


typedef unsigned int Index;

Index Hash(const char *key, const int TableSize)
{
	unsigned int HashVal = 0;
	while (*key != '\0'){
		HashVal += *key++;
	}

	return HashVal % TableSize;
}

Index HAsh(const char *key, const int TableSize)
{
	unsigned int HashVal = 0;
	while (*key != '\0'){
		HashVal = (HashVal << 5) + *key++;
	}
	return HashVal % TableSize;
}
