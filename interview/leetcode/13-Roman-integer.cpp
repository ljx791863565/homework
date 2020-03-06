/*
**leetcode No. 13
**Roman to integer
	I	1	II	2	III	3	IV	4	VI	6	VII	7	VIII	8	IX	9
	V	5
	X	10
	L	50
	C	100
	D	500
	M	1000
 */
#include <iostream>
#include <unordered_map>
using std::unordered_map;
using namespace std;
class Solution
{
public:

	int RomanToInt(string s)
	{
		unordered_map<char, int> T = {
			{'I', 1},
			{'V', 5},
			{'X', 10},
			{'L', 50},
			{'C', 100},
			{'D', 500},
			{'M', 1000}
		};
		int res = T[s.back()];	//s.back()返回最后一个字符的引用，不可改变
		for (int i = s.length() - 2;  i >= 0; --i){
			if (T[s[i]] < T[s[i+1]]){
				res -= T[s[i]];
			} else {
				res += T[s[i]];
			}
		}
		return res;
	}
};

int main()
{
	string elem;
	cin >> elem;
	Solution s;
	cout << elem << " is  " << s.RomanToInt(elem) << endl;

	return 0;
}
