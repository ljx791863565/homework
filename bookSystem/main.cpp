#include "book.h"

using namespace std;

int main(int argc, const char *argv[])
{
	Book a;
	Book b("高等代数", "Tomson", "机械工业出版社", "adfdbgf");
	Book c(b);

	cout << b.getBookName() << endl;
	cout << c.getISBN() << endl;
	return 0;
}
