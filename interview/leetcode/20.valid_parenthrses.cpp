
/*
   输入一个字符串，判断是否是完整的符号对应
   */

#include <iostream>
#include <stack>
using namespace std;
class Solution
{
public:
	bool isValid(string s)
	{
		stack<char> s1;
		//遍历所有字符串字符，当存在和栈对应字符，则弹栈并压入下一个
		//当所有符号一一对应是，栈为空
		for (int i=0; i<s.size(); i++){
			if (s1.empty())
				s1.push(s[i]);
			else if (s1.top() == '{' && s[i] == '}')
				s1.pop();
			else if (s1.top() == '(' && s[i] == ')')
				s1.pop();
			else if (s1.top() == '[' && s[i] == ']')
				s1.pop();
		}
		//当栈为空时，说明符号一一对应
		if (s1.empty())
			return true;
		else
			return false;
	}
private:
	int val;
};

int main()
{
	Solution s;
	string str;
	cin >> str;
	if (s.isValid(str))
		cout << str << "is true" << endl;
	else
		cout << str << "is false" << endl;


	return 0;
}
