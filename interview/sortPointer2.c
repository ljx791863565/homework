#include <stdio.h>
#include <string.h>

//在二维数组做参数中可以退化为(*)arr[20] 需要告诉编译器这个数组的列数。行数可以退化成指针，所以不需要
//至于退化成这个之后不知道怎么调用，报错 先拍砖在这里，待解释
void printArray2(char arr[][20], int num)
{
	int i;
	for (i=0; i<num; ++i){
		printf("%s ", arr[i]);
	}
	printf("\n");
}

//二级指针做输入的第二种模型，用在二维数组中

void sortArray2(char arr[][20], int num)
{
	int i = 0, j = 0;
	char tmp[20] = {0};
	//交换的是二维数组的值，所以要用strcpy
	for (i =0; i<num; ++i){
		for (j=i; j< num; j++){
			if (strcmp(arr[i], arr[j]) > 0){
				strcpy(tmp, arr[i]);
				strcpy(arr[i], arr[j]);
				strcpy(arr[j], tmp);
			}
		}
	}
}
int main()
{
	//二维数组
	char phone[10][20] = {
		"Apple", "Samsung", "Oppo", "Xiaomi", "Meizu", "Nokia"
	};
/*
	int num = 0;
	for (; strcmp(phone, "0"); phone = phone+1){
		num++;
	}
*/
	printf("num = %d\n", num);
	//int num = sizeof(phone) / sizeof(phone[0])
	//当数组元素不满时，初始化之后的值会被编译器自动赋值0
	//不可以用sizeof求二维数组元素个数，因为永远会是行数10
	printf("排序前\n");
	printArray2(phone, 6);

	sortArray2(phone, 6);

	printf("排序后\n");
	printArray2(phone, 6);
	return 0;
}
