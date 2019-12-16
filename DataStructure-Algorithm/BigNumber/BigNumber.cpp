/*************************************************************************
    > File Name: BigNumber.cpp
    > Author: ljx
    > Mail: liaojia138797@163.com 
    > Created Time: Mon 16 Dec 2019 02:37:32 PM CST
 ************************************************************************/
#include "BigNumber.h"

BigNum::BigNum(INT_64 value)
	:_value(value)
{
	//long long int 最大19位，符号位一位，缓冲一位，申请扩大到21位
	_strData.resize(21, '0');
	//使字符串带上数字的正负符号
	sprintf((char *)_strData.c_str(), "%+lld", value);
}

BigNum::BigNum(const char *num_str)
	:_value(0)
{
	if (NULL == num_str){
		return ;
	}
	char *str = (char *)num_str;
	//删除空格
	while (isspace(*str)){
		++str;
	}
	char symbol = '+';
	if ('-' == *str || '+' == *str){
		symbol = *(str++);
	}
	//删除开始的0字符
	while ('0' == *str){
		++str;
	}
	_strData.resize(strlen(str) +1);

	char *dst = (char *)_strData.c_str();
	*(dst++) = symbol;		//*dst = symbol; dst++;

	while (*str >= '0' && *str <= '9'){
		_value = _value *10 + *str - '0';		//ljx： 如果超过了long long的最大值怎么办?
		*(dst++) = *(str++);
	}
	if ('-' == symbol){
		_value = 0 - _value;	
	}

	_strData.resize(strlen(_strData.c_str()));

	if (_strData.size() == 1){
		_strData.append(1, '0');
	}
}

BigNum::BigNum(const BigNum &big)
{

	_value = big._value;
	_strData = big._strData;
}

BigNum & BigNum::operator=(const BigNum &big)
{

	if (this != &big){
		_value = big._value;
		_strData = big._strData;
	}
	return *this;
}

//true: 超过long long的范围
//flase：未超过范围
bool BigNum::IsINT64Overflow() const
{

	//最大范围为19位，但是有一位符号位
	if (_strData.size() > 20){
		return true;
	}
	if (_strData.size() == 20){
		string max("9223372036854775807");
		string min("-9223372036854775808");
		char *tmp = (char *)_strData.c_str();
		if ('-' == tmp[0]){
			return _strData < min;
		}else{
			return _strData > max;
		}
	}

	return false;
}

ostream &operator<<(ostream &os, BigNum &big)
{

	if (!big.IsINT64Overflow()){
		os << big._value;
	}else{
		const char *tmp = big._strData.c_str();
		if ('+' == *tmp){
			++tmp;
		}
		//这里如果是负数直接输出，如果是正数，需要去掉+
		cout << tmp;
	}
}

BigNum operator+(const BigNum &left, const BigNum &right)
{

	//1.左右操作数均未溢出
	if (!left.IsINT64Overflow() && !right.IsINT64Overflow()){

		//同号且溢出情况
		if (((left._value > 0 && right._value > 0) && (INT_64_MAX - left._value < right._value))
				|| (left._value < 0 && right._value < 0) && (INT_64_MIN - left._value > right._value)){
			//这里不可以直接相加
		}
		//不会溢出
		else {
			//直接相加并且返回成一个BigNum对象
			return BigNum(left._value + right._value);
		}
	}
	//2.左右操作数有一个溢出或两个都溢出
	return BigNum(Add(left._strData, right._strData).c_str());
}

string Add(const string &left, const string &right)
{
	//c风格字符串
	const char *Left = left.c_str();
	const char *Right = right.c_str();
	int LSize = strlen(Left);
	int RSize = strlen(Right);

	char symbol;
	//同号
	if (*Left == *Right){

		if (LSize < RSize){
			swap(LSize, RSize);
			swap(Left, Right);
		}

		symbol = *Left;
		string result;
		//默认和最大为较大数+1位，负数时为最小数+1位
		result.resize(LSize+1);

		char *ret = (char *)result.c_str();
		ret[0] = symbol;

		//进位值
		int carry = 0;
		int i;
		for (i=1; i<LSize; ++i){
			int sum;
			if (i < RSize){
				sum = Left[LSize - i] - '0' + Right[RSize - i] - '0' + carry;
			}else {
				//此时只需要加上较长数剩余位数，较短数已经加完
				sum = Left[LSize - i] - '0' + carry;
			}
			carry = sum / 10;
			sum %= 10;	//进位后余下的值为最终值
			ret[LSize - i + 1] = sum + '0';
		}
		//最大位可能进位时，需要补上此值
		ret[1] = carry + '0';
		return result;
	}
	//异号，转换为同号相减
	else {

		string l(Left);
		string r(Right);

		if ('-' == *Left){
			char *tmp = (char *)l.c_str();
			tmp[0] = '+';
			return Sub(r, l);	//转换为同正号相减
		}else {
			char *tmp = (char *)r.c_str();
			tmp[0] = '+';
			return Sub(l, r);
		}
	}
}

BigNum operator-(const BigNum &left, const BigNum &right)
{

	//1.左右操作数均未溢出
	if (!left.IsINT64Overflow() && !right.IsINT64Overflow()){

		//同号且溢出情况
		if (((left._value > 0 && right._value > 0) && (INT_64_MAX - left._value < right._value))
				|| (left._value < 0 && right._value < 0) && (INT_64_MIN - left._value < right._value)){

			//do nothing
		}
		//不会溢出
		else {

			return BigNum(left._value - right._value);
		}
	}
	//2.左右操作数有一个溢出或两个都溢出
	return BigNum(Sub(left._strData, right._strData).c_str());
}

string Sub(const string &left, const string &right)
{
	string left_tmp = left;

	char *Left = (char *)left_tmp.c_str();
	char *Right =(char *)right.c_str();

	int LSize = strlen(Left);
	int RSize = strlen(Right);

	char symbol;
	//同号
	if (*Left == *Right){

		//计算最终结果符号
		symbol = '-';
		if ('+' == *Right){

			if ((left.size() > right.size() || (left.size() == right.size() && left > right))){

				symbol = '+';
			}
		}else {
			//两个负数相减时，绝对值较小的减去绝对值较大的结果为正
			if ((left.size() < right.size()) || (left.size() == right.size() && left < right)){

				symbol = '+';
			}
		}

		//其余情况结果为-
		
		//让被减数在下边
		if (LSize < RSize){
			swap(LSize, RSize);
			swap(Left, Right);
		}

		string result;
		result.resize(LSize);
		char *dst = (char *)result.c_str();
		dst[0] = symbol;

		int carry = 0;
		int sub = 0;
		int i;
		for (i=1; i<LSize; ++i){
			Left[LSize - i] -= carry;	//被借位后需要减去借位值

			//从尾部向头部逐个相减
			if (i < RSize){
				sub = Left[LSize - i] -  Right[RSize - i];
			}else {
				sub = Left[LSize - i] - '0';
			}


			if (sub < 0){
				//如果前一位有值时，向前一位接位1，后一位需要+10
				if (left[LSize - i - 1] >= '0' && left[LSize - i - 1] <= '9'){
					carry = 1;
					sub += 10;
				}else {
					sub = 0 - sub;	//前面没有值可以供借位，将值变成正数
				}
			}else {
				carry = 0;		//值为正数，不需要借位
			}

			dst[LSize - i] = sub + '0';		//保存在结果位
		}
		return result;
	}
	//异号
	else {

		if ('+' == *Left){

			string r(right);
			char *tmp = (char *)r.c_str();
			tmp[0] = '+';
			return Add(left, r);
		}else {

			string l(left);
			char *tmp = (char *)l.c_str();
			tmp[0] = '+';
			string ret = Add(l, right);
			tmp = (char *)ret.c_str();
			if ('-' == *tmp){

				*tmp = '+';
			}else {

				*tmp = '-';
			}
			return ret;
		}
	}
}

