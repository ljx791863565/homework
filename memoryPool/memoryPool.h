#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__

#include <iostream>
#include <mutex>

#define MAX_MEMORY_SIZE 256

class MemoryPool;

struct MemoryBlock
{
	MemoryBlock *pNext;
	bool bUsed;
	bool bBelong;
	MemoryPool *pMem;
};

class MemoryPool
{
public:
	MemoryPool(size_t nSize = 128, size_t nBlock = 10)
	{
		_nSize = nSize;
		_nBlock = nBlock;
		_pHeader = NULL;
		_pBuffer = NULL;
	}
	virtual ~MemoryPool()
	{
		if (_pBuffer != NULL) {
			free(_pBuffer);
		}
	}
	void* AllocMemory(size_t nSize)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		//指向第一块内存的指针为空，表示没有申请任何内存
		if (_pBuffer == NULL) {
			InitMemory();
		}
		MemoryBlock *pRes = NULL;
		if (_pHeader == NULL) {
			pRes = (MemoryBlock *)malloc(nSize + sizeof(MemoryBlock));
			pRes->bBelong = false;
			pRes->bUsed = false;
			pRes->pNext = NULL;
			pRes->pMem = NULL;
		}else {
			pRes = _pHeader;
			_pHeader = _pHeader->pNext;
			pRes->bUsed = true;
		}
		return ((char*)pRes + sizeof(MemoryBlock));
	}

	void FreeMemory(void *p)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		MemoryBlock *pBlock = ((MemoryBlock*)p - sizeof(MemoryBlock));
		if (pBlock->bBelong) {
			pBlock->bUsed = false;
			pBlock->pNext = _Header;
			pBlock = _Header;
		}else {
			free(pBlock);
		}
	}

	void InitMemory()
	{
		if (_pBuffer) {
			return;
		}
		size_t PoolSize = _nSize + sizeof(MemoryBlock);
		size_t BuffSize = PoolSize * _nBlock;
		_pBuffer = (char *)malloc(BuffSize);
		_pHeader->bUsed = false;
		_pHeader->bBelong = true;
		_pHeader->pMem = this;

		MemoryBlock *tmp1 = _pHeader;
		for (size_t i = 1; i < _nBlock; i++) {
			MemoryBlock *tmp2 = (MemoryBlock *)(_pBuffer + i*PoolSize);
			tmp2->bUsed = false;
			tmp2->bBelong = true;
			tmp2->pNext = NULL;
			_pHeader->pMem = this;
			tmp1->pNExt = tmp2;
			tmp1 = tmp2;
		}

	}
public:
	/*
	 *  MemoryPool:
	 *	---------------------------------------------------------------------
	 *	|_nSize	|_nBlock|_pBuffer|_pHeader|_mutex	|			...			|
	 *	---------------------------------------------------------------------
	 *								|	
	 *	--<------<-------<----------|
	 *	|
	 *	|
	 *	---------------------------------
	 *	|bUsed	|bBelong|pNext	|pMem	|
	 *	---------------------------------
	 */
	char			*_pBuffer;	//指向第一块内存块的地址
	MemoryPool		*_pHeader;	//指向内存块头指针
	size_t			_nSize;		//每块内存块大小
	size_t			_nBlock;	//内存块数量
	std::mutex		_mutex;
};
#endif //__MEMORY_POOL_H__
