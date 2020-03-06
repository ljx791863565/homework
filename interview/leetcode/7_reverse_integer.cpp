#include <climits>
#include <cstdio>
#include <stdint.h>
//leetcode No. 7
//要求给定一个32bit的数返回其反数
//eg:
//	123->321
//	120->21
//	-234->-543
class Solution
{
public:
	int reverse(int x)
	{
		const int R = 10;
		if (x == INT_MIN){
			return 0;
		}
		if (x < 0){
			return -reverse(-x);
		}
		int64_t res = 0;
		while (x > 0){
			res = res * R + x % R;
			if (res > INT_MAX){
				return 0;
			}
			x /= R;
		}
		return res;
	}
};

int main()
{
	int a = -345;
	
	Solution s;
	int64_t num = s.reverse(a);
	printf("num = %ld\n", num);

	return 0;
}
