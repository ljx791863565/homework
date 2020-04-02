char foo(void)
{
	unsigned int a = 6;
	int b = -20;
	if (a + b > 6) {
		printf("> 6\n");
	}else{
		printf("<6\n");
	}
}

/*
 * unsigned int 和 int 类型数据运算时 int类型会自动转换成unsigned int类型 所以b会变成一个很大的数 相加会>6
 *
 * 类型转换的几种情况
 * 1.混合类型的算术表达式中
 * int ival = 3;
 * double dval = 3.14;
 * ival + dval ; 这里表达式中ival会被升为double类型3.0
 *
 * 2.一种类型赋值给另一种类型
 * int *p = 0;	0被转换成指针类型值 表示为空指针
 * ival = dval;	dval赋值给ival会转换成int类型 将会截断成3 导致数据失真 
 * dval = ival; ival赋值给dval时会转换成double 3.000000
 * 所以为了保证数据的精度 如果可以的话都要求类型转换为更宽的类型
 * 所有小于整形的类型在计算时都会转换为整形
 *
 * 3.调用函数参数中转换
 * void foo(double d);
 * foo(2);	2会被转换为double类型  2.000000
 *
 * 4.函数返回值转换
 * double foo(int a, int b)
 * {
 *	//do something
 *	return a + b;
 * }
 *
 *
 *
 */
