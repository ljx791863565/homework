/*
   函数查找字符串数组中最长的公共字符前缀，返回此字符串，如果没有，返回""
   */

#include <iostream>
#include <vector>
using namespace std;
class Solution
{
public:
	string longestCommonPrefix(vector<string>& strs)
	{
		int n = strs.size();
		if (n == 0){
			return "";
		}
		int min_val = strs[0].size();	//按照第一个string来确定前缀字符串的大小
		string res = "";

		for (int i=0; i<min_val; ++i){
			for (int j=1; j<n; ++j){
				if (strs[0][i] != strs[j][i]){
					return res;
				}
				if (min_val > strs[j].size()){
					min_val = strs[j].size();
				}
			}
			res += strs[0][i];
		}
		return res;
	}
};

int main()
{
	vector<string> str;
	string tmp;
	for (int i = 0; i < 3; ++i){
		cin >> tmp;
		str.push_back(tmp);
	}

	for (int i = 0; i < 3; ++i){
		cout << str[i] << " ";
	}
	cout << endl;
	Solution s;
	string prefix = s.longestCommonPrefix(str);
	cout << "this longest commin prefix is : " << prefix << endl;

	return 0;
}
