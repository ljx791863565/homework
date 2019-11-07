#include <iostream>
#include <string.h>
#include <assert.h>

//找出字符串中子串出现的位置，如果找不到返回NULL
char *BF(const char *string, const char *sunstring)
{
	assert(string != NULL && substring != NULL);
	int m = strlen(string);
	int n = strlen(substring);
	if (m < n){
		return NULL;
	}
	int i, j;
	//此算法的时间复杂度是 O((m-n-1)*n)
	//i表示最少要比较m-n次才能保证找到有n个字符的子串
	for (i =0; i< m-n; i++){
		for (j = 0; j<n; j++){
			//在子串内部一旦找到不相等的字符立刻跳出表示此子串不符合条件
			if (string[i+j] != substring[j]){
				break;
			}
		}
		//此时从i开始j个字符都等于substring，所以找到了子串
		if (j == n){
			return string+j;
		}
	}
	//表示没找到
	return NULL:
}
