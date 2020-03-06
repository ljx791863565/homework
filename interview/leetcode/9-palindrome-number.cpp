//leetcde No.9
//回文数字

#include <cstdio>

class Solution {
public:
    bool isPalindrome(int x) {
        if (x < 0) {
            return false;
        }
		if ((x % 10) == 0 && x != 0){
			return false;
		}
        int res = 0;
        while (x > res) {
            res = res * 10 + x % 10;
            x /= 10;
        }
        return res == x || x == res/10;
    }
};

int main()
{
	Solution s;
	int a = 123, b = 1221;
	if (s.isPalindrome(b)){
		printf("%d is palindrome\n", b);
	}else{
		printf("%d is not palindrome\n", b);
	}
	
	return 0;
}
