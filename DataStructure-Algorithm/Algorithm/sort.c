#include <stdio.h>
#include <time.h>
#include <stdlib.h>
clock_t start, stop;
double duration;
void show(int *arr, int n)
{
	for (int i=0; i<n; ++i){
		printf("%d ",arr[i]);
	}
	printf("\n");
}

//选择排序
void SelectionSort(int *arr, int n)
{
	for (int i=0; i<n; ++i){
		for (int j = i+1; j<n; ++j){
			if (arr[i] > arr[j]){
				int tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

//冒泡排序
void BubbleSort(int *arr, int n)
{
	for (int i=0; i<n; ++i){
		for (int j=0; j<n-i-1; ++j){
			if (arr[j] > arr[j+1]){
				int tmp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tmp;
			}
		}
	}
}

//桶排序
void BucketSort(int *arr, int n)
{
	int max = arr[0];
	for (int i=1; i<n; ++i){
		if (arr[i] > max){
			max = arr[i];	//找出需要最大的空间
		}
	}
	printf("max = %d\n", max);
	int *p = (int*)malloc((max+1)*(sizeof(int)));	//申请max+1 个 int大小的内存空间
	for (int i=0; i<max+1; ++i){
		p[i] = 0;	//清空		
	}
	for (int i=0; i<n; ++i){
		p[arr[i]]++;//计算每个元素出现的次数
	}
	for (int i=0,j=0; i<max+1; ++i){
		while(p[i] != 0){
			arr[j++] = i;
			p[i]--;
		}
	}

	free(p);
}

//快速排序
//一般来说非常快
//时间复杂度 O(N logN)
//low 数组下限		high 数组上限
void QuickSort(int *arr, int low, int high)
{
	int mid = arr[low];
	while (low < high){
		while (low < high && arr[high] >= mid){
			high--;
		}
		arr[low] = arr[high];	//比中轴小的移动到左端

		while (low < high && arr[low] < mid){
			low++;
		}
		arr[high] = arr[low];	//比中轴大的移动到右端
	}
	arr[low] = mid;
	return ;
}

/*
//递归的快速排序
void sort4_1(int *arr, int low, int high)
{
while (low < high){
int mid = sort4(arr, low, high)
}
}
*/

//插入排序
//时间复杂度 O(n^2)
void InsertionSort(int *arr, int n)
{
	int tmp = 0;
	int i, j;
	for (i=1; i<n; i++){
		tmp = arr[i];
		//假如tmp比前面的值小，将tmp前面的值后移
		for (j=i; j>0 && tmp < arr[j-1]; j--){
			arr[j] = arr[j-1];
		}
		arr[j] = tmp;
	}
}

void ShellSort(int *arr, int n)
{
	int i, j, Increment;
	int tmp;
	for (Increment = n / 2; Increment > 0; Increment /= 2){
		for (i = Increment; i < n; i++){
			tmp = arr[i];
			for (j = i; j >= Increment; j -= Increment){
				if (tmp < arr[j - Increment]){
					arr[j] = arr[j - Increment];
				}else {
					break;
				}
			}
			arr[j] = tmp;
		}
	}
}
int main()
{
	int n, i;
	printf("Input n\n");
	scanf("%d", &n);
	int arr[n];
	srand((unsigned)time(NULL));
	for (i=0; i<n; i++){
		arr[i] = rand()%100000 +1;
	}
	show(arr,n);
	printf("after sotr: \n");
	start = clock();
//	InsertionSort(arr, n);
	ShellSort(arr, n);
	stop = clock();
	show(arr,n);
	duration = ((double)(stop - start)) /CLOCKS_PER_SEC;

	printf("durations = %.2f\n", duration);

	return 0;
}
