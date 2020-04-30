#ifndef BUFFER_H
#define BUFFER_H 
#include <string>
#include <string.h>
#include <vector>
#include <assert.h>

//		buffer_
//		--------------------------------------------
//      |		|					|				|		
//      |		|					|				|
//      --------------------------------------------
//      |		|					|				|
//		0		readindex_		writeindex_			size()
//
//		^		^					^				^
//		|		|					|				|
//		perpendable
//						readable				
//										writeable
//
//		perpednable = readable
//		readable = wirteindex_ - readindex_
//		writeable = size() - wirteindex_
//		
//		0 <= readindex_ <= wirteindex_ <= size()
//
//		初始化 1024bit
//		-------------------------------------------------
//		|		|										|
//		-------------------------------------------------
//		0		8										1032
//			readindex_
//			writeindex_
class Buffer
{
	static const size_t kCheapPrepend = 8;
	static const size_t kInitialSize = 1024;
public:
	//初始化1kb大小，1024字节，前面预留8字节用于后续可以方便记录大小
	//一开始没有数据，所以readindex_=writeindex_ 
	//readable = 0  readable = writeindex - readindex
	//writeable = size - writeindex
	Buffer(int initialSize = kInitialSize) 
		: buffer_(initialSize+8),
		readIndex_(kCheapPrepend),
		writeIndex_(kCheapPrepend) 
	{
		assert(readableBytes() == 0);
		assert(writeableBytes() == initialSize);
		assert(perpendableBytes() == kCheapPrepend);
	}

	void swap(Buffer& rhs)
	{
		buffer._swap(rhs.buffer_);
		std::swap(readindex_, rhs.readindex_);
		std::swap(writeindex_, rhs.writeindex_);
	}

	//read
	void retrieve(size_t len) {
		assert(len <= readableBytes());
		if (len < readableBytes()) {
			readIndex_ += len;
		} else {
			retrieveAll();
		} 
	}

	//read all 
	void retrieveAll() {
		readIndex_ = kCheapPrepend;
		writeIndex_ = kCheapPrepend;
	}

	char* begin() {
		return &*buffer_.begin();
	}

	const char* begin() const {
		return &*buffer_.begin();
	}

	const char* peek() const {
		return begin() + readIndex_;
	}

	int16_t peekInt16() {
		assert(readableBytes() >= sizeof(int16_t));
		int16_t result = 0;
		memcpy(&result, peek(), sizeof(result));
		return result;
	}

	int16_t readInt16() {
		int16_t result = 0;
		result = peekInt16();
		retrieve(sizeof(result));
		return result;
	}

	char* beginWrite() {
		return begin() + writeIndex_;
	}

	size_t readableBytes() const {
		return writeIndex_ - readIndex_ ;
	}

	size_t writeableBytes() const {
		return buffer_.size() - writeIndex_;
	}

	size_t prependableBytes() const {
		return readIndex_;
	}

private:
	std::vector<char> buffer_;		
	size_t readIndex_;
	size_t writeIndex_;
	
}

#endif
