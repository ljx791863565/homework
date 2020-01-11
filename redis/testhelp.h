#ifndef _TESTHELP_H_
#define _TESTHELP_H_

#include <stdlib.h>
int __failed_tests = 0;
int __test_num = 0;
//测试函数
//第一个参数是要测试的问题
//第二个参数是测试条件
//如果测试条件为真 测试PASSED
//如果测试条件为假 测试FAILED 
//test_cond("Check if 1 == 1", 1==1);
//test_cond("Check if 5 > 10", 5 > 10);
//test_report();
#define test_cond(desct, _c) do { \
	__test_num++;\
	printf("%d - %s: ",__test_num, desct);\
	if (_c)\
		printf("PASSED\n");\
	else \
		printf("FAILED\n"); __failed_tests++; \
} while (0);

//结果的公布函数
#define test_report() do { \
	printf("%d tests, %d passed, %d failed\n", __test_num, (__test_num - __failed_tests), __failed_tests); \
	if (__failed_tests) { \
			printf("=== 测试结果中存在错误... ===\n"); \
		exit(1); \
	} \
} while(0);
#endif	//_TESTHELP_H_
