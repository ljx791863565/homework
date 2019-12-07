/*
有N个数返回第K大的数	
*/

// O(n^2)
int kThNumber(int *array, int n, int k)
{
	BobbleSort(array, n);
	return array[n-k];
}

int kThNumber2(int *array, int n, int k)
{
	int i, j;
	for (i = 0; i <k; i++){
		for (j = 0; j < k-1-i; j++){
			if (array[j] <array[j+1]){
				swap(array[j], array[j+1]);
			}
		}
	}

	for (i=k+1; i < n; i++){
		if (array[i] > array[k]){
			
		}
	}
}
