#include <stdio.h>
#include <string.h>

void printArray1(char **arr, int num)
{
	int i;
	for (i=0; i<num; ++i){
		printf("%s ", arr[i]);
	}
	printf("\n");
}

//二级指针做输入的第一种模型，用在指针数组中

void sortArray1(char **arr, int num)
{
	int i, j;
	//我们只交换指针的值，而不交换指针指向的内存空间的值
	//指针的值和指针指向内存空间的值是两个概念
	//指针也是一种变量
	for (i =0; i<num; ++i){
		for (j=i; j< num; j++){
			if (strcmp(arr[i], arr[j]) > 0){
				char *tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}
int main()
{
	//指针数组 从右向左看，首先phone是一个数组，元素是指针
	char *phone[] = {
		"Apple", "Samsung", "Oppo", "Xiaomi", "Meizu", "Nokia"
	};
	int num = sizeof(phone) / sizeof(phone[0]);
	printf("排序前\n");
	printArray1(phone, num);

	sortArray1(phone, num);

	printf("排序后\n");
	printArray1(phone, num);
	return 0;
}
