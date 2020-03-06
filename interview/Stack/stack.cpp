#include "stack.h"
using namespace std;

static void PrintValue(const int nRetCode, const int nValue)
{
	if (nRetCode)
		cout << nValue << endl;
	else
		cout << "Error occured" << endl;
}

int main()
{
	CStack<int> stack;
	int nValue;
	int nRetCode;

	stack.push(1);
	stack.push(2);
	stack.push(3);

	nRetCode = stack.top(&nValue);
	PrintValue(nRetCode, nValue);
	
	nRetCode = stack.pop(&nValue);
	PrintValue(nRetCode, nValue);

	nRetCode = stack.pop(&nValue);
	PrintValue(nRetCode, nValue);

	nRetCode = stack.pop(&nValue);
	PrintValue(nRetCode, nValue);

}
