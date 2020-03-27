#ifndef	_BOOK_H_
#define _BOOK_H_

#include <iostream>
#include <string>

using namespace std;
class Book{
public:
	Book()
		:bookName(""), author(""), publishingHouse(""), ISBN("")
	{
	}
	Book(const Book &b)
	{
		bookName = b.bookName;
		author = b.author;
		publishingHouse = b.publishingHouse;
		ISBN = b.ISBN;
	}
	Book(const string &name, const string &au, const string &press, const string &isbn)
	{
		bookName = name;
		author = au;
		publishingHouse = press;
		ISBN = isbn;
	}
	string& getBookName();
	string& getAuthor();
	string& getPublishingHouse();
	string& getISBN();

	void setBookName(string &b);
	void setAuthor(string &a);
	void setPublishingHouse(string &p);
	void setISBN(string &i);
	

private:
	string bookName;
	string author;
	string publishingHouse;
	string ISBN;
};

#endif
