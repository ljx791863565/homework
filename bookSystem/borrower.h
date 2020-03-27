#ifndef _BORROWER_H_
#define _BORROWER_H_

#include "book.h"

class Borrower
{	
public:
	Borrower(string &name, string &gender, int age, string &id)
		:name(name), gender(gender), age(age), ID(id), balance(0.0)
	{}
	float getBalance();
	void rechageBalance(float money);
	Book& findByBookName(string &name);
	Book& findByAuther(string &au);
private:
	string name;
	string gender;
	int age;
	string ID;
	float balance;
};
#endif

