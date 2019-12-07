#include <stdio.h>
void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b; 
	*b = temp;
}

int BubbleSort(int *array, int n)
{
	int i, j;
	for (i = 0; i <n; i++){
		for (j = 0; j < n-i-1; j++){
			if (array[j] > array[j+1]){
				swap(&array[j], &array[j+1]);
			}
		}
	}
}

void display(int *array, int n)
{
	int i;
	for (i = 0; i < n; i++){
		printf("%4d ", array[i]);
	}
	printf("\n");
}
