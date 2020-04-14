#include <stdio.h>

/*
 * 一个数组中一个数出现一次其他数出现两次，找出这个数
 * 一个数组中两个数出现一次其他数出现两次，找出这两个数
 * 一个数组中三个数出现一次其他数出现两次，找出这三个数
 */


// a^0=a a^a=0
int singleNumber(int array[], int n)
{
	int i = 0;
	int tmp = 0;
	for (i=0; i<n; i++){
		tmp ^= array[i];
	}
	return tmp;
}

/*
 * 对于出现两次的元素，使用“异或”操作后结果肯定为0，那么我们就可以遍历一遍数组，对所有元素使用异或操作，那么得到的结果就是两个出现一次的元素的异或结果。
 * 因为这两个元素不相等，所以异或的结果肯定不是0，也就是可以再异或的结果中找到1位不为0的位，例如异或结果的最后一位不为0。
 * 这样我们就可以最后一位将原数组元素分为两组，一组该位全为1，另一组该位全为0。
 * 再次遍历原数组，最后一位为0的一起异或，最后一位为1的一起异或，两组异或的结果分别对应着两个结果。
 *
 * 时间复杂度 O(n) + O(32) + O(n) 即为O(n)
 * 空间复杂的 常数
 *
 * 另一个思路是构建一个map key为数组元素 value为该元素出现的次数 遍历一次数组即可得到数组所有元素出现的次数】
 * 时间复杂度 遍历数组构建map O(n) 遍历map O(n/2) 即为O(n)
 * 空间复杂度  构建map O(n/2) 即 O(n)
 * void doubleNumber(int array[], int n)
 * {
 *		std::map<int, int> hsahMap = new hashMap<int, int>();
 *		for (int i : array) {
 *			if (hashMap.count(i)) {
 *				//i存在
 *				std::map<int, int>::iterator it_find;
 *				it_find = hashMap.find(0);
 *				it_find->second ++;
 *			}else {
 *				//i不存在
 *				hashMap.insert(make_pair(i, 1));
 *			}
 *		}
 *		std::map<int, int>::iterator it;
 *	    for (it : hashMap){
 *			
 *	    }
 *
 * }
 */
void doubleNumber(int array[], int n)
{
	int i = 0;
	int tmp = 0;
	int num1 = 0, num2 = 0;
	int index = 0;
	for (i = 0; i < n; i++) {
		//这时候tmp为只出现一次的两个数的异或值 由于有两个数不同，其异或值必定有某个bit为1
		tmp ^= array[i];
	}

	for (i = 0; i < 32; i++) {
		//找到异或后结果从高位到低位二进制中最先出现1的位置
		if (((tmp >> i) & 1) != 1) {
			index++;
		}else {
			break;
		}
	}
	
	//根据第index位是不是1把array分为两组 第一个子组每一个数的第index都是1 第二个子组每一
	//数的第index位都是0 每个子组都有一个数只出现了一次其余数各出现了两次
	for (i = 0; i < n; i++) {
		if (((array[i] >> index) & 1) == 1) {
			num1 ^= array[i];
		}else {
			num2 ^= array[i];
		}
	}

	printf("%d %d\n", num1, num2);
}

void trebleNumber(int array[], int n)
{
//https://www.xuebuyuan.com/1176737.html
//https://www.cnblogs.com/CarrieCui/p/5119597.html
}
int main()
{
	int array[7] = {1, 3, 5, 6, 1, 3};
//	int num = singleNumber(array, 7);
//	printf("%d\n", num);
	doubleNumber(array, 6);
	return 0;
}
