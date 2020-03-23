#include <stdio.h>

//二分查找必须建立在数组升序排序的情况下
//如果找到了 返回这个数在数组中的下标
//如果没有找到 返回-1
int BinarySearch(int array[], int key, int n)
{
	int low, high, mid;
	low = 0;
	high = n-1;

	while (low <= high){
		mid = (low + high) / 2;
		if (key < array[mid]){
			high = mid - 1;
		}else if (key > array[mid]){
			low = mid + 1;
		}else {
			return mid;
		}
	}
	return -1;
}
int main()
{
	int array[10] = {
		3, 9, 23, 47, 50, 113, 369, 450, 1138, 3457
	};
	int key = 113;
	int n = BinarySearch(array, key, 10);

	if (-1 == n){
		printf("no number\n");
	}else {
		printf("%d in array index %d\n", key, n);
	}
	return 0;
}
