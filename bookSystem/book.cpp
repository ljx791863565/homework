#include "book.h"

string& Book::getBookName()
{
	return bookName;
}
string& Book::getAuthor()
{
	return author;
}
string& Book::getPublishingHouse()
{
	return publishingHouse;
}
string& Book::getISBN()
{
	return ISBN;
}

void Book::setBookName(string &b)
{
	bookName = b;
}
void Book::setAuthor(string &a)
{
	author = a;
}
void Book::setPublishingHouse(string &p)
{
	publishingHouse = p;
}
void Book::setISBN(string &i)
{
	ISBN = i;
}
