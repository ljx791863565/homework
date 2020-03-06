#ifndef _STRING__H__INCLUDE
#define _STRING__H__INCLUDE

#include <cstring>
class String
{
friend ostream& operator<<(ostream &os, const String &s);
friend istream& operator>>(istream &in, String &s);
public:
	String();
	String(const String& s);
	String(const char *pc, size_t len);
	String(const char *pc);
	~String();

	String& operator=(const String &s);
	String& operator=(const char *s);

	String& operator+=(const String &rhs);
	String& operator+=(const char *rhs);

	void clear();
	size_t getLength() const 
	{
		return _length;
	}
	const char* cstr() const 
	{
		return _cstr;
	}

private:
	void _initString(const char *cstr, size_t len);
	void _decUsed();
	char* _renewAndCat(const char *cstr, size_t len);
	void _addString(const char *cstr, size_t len);
	void _addAssignOpt(const char *cstr, size_t len);

private:
	char *_cstr;
	size_t *_used;
	size_t _length;
	size_t _capacity;
};

String operator+(const String &lhs, const String &rhs);


#endif //_STRING__H__INCLUDE
