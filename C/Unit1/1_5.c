
#include <stdio.h>

float FtoC(float fahr)
{
	float celsius;
	celsius = (5.0 / 9.0)* (fahr - 32.0);
	return celsius;
}
float CtoF(float celsius)
{
	float fhar;
	fhar = (9.0 /5.0 )*celsius + 32.0;
	return fhar;
}
int main()
{
	float lower = 0;
	float upper = 100;
	float i;
	printf("Celsius to Fhar\n");
	for (i=upper; i>=lower; i-=20){
		printf("%3.1f\t%3.1f\n",i, CtoF(i));
	}
	printf("\n");
	printf("Fhar to Celsius\n");
	for (i=upper; i>=lower; i-=20){
		printf("%3.1f\t%3.1f\n",i, FtoC(i));
	}

	return 0;
}
