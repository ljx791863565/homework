#include <stdio.h>
#include <stdlib.h>
#define MAX_DEGREE 1000
//二项式
typedef struct polynomial{
	int coefficient_array[MAX_DEGREE +1];	//数组保存各项系数
	int HighPower;	//最高次幂
}Polynomial_t;

void ZeroPolynomial(Polynomial_t poly)
{
	int i;
	for (i=0; i<MAX_DEGREE+1; i++)
		poly->coefficient_array[i] = 0;
	poly->HighPower = 0;
}

void AddPolynomial(Polynomial_t poly1, Polynomial_t poly2, Polynomial_t polySum)
{
	int i;
	ZeroPolynomial(polySum);

	for (i=0; i<MAX_DEGREE+1; i++)
		polySum->coefficient_array[i] = poly1->coefficient_array[i] + poly2->coefficient_array[i];
	polySum->HighPower = poly1->HighPower > poly2->HighPower ? poly1->HighPower : poly2->HighPower;
}

void MilPolynomial(Polynomial_t poly1, Polynomial_t poly2, Polynomial_t polyProduct)
{
	ZeroPolynomial(polyProduct);
	polyProduct->HighPower = poly1->HighPower + poly2->HighPower;
	if (polyProduct->HighPower > MAX_DEGREE)
		printf("乘积溢出\n")；
	int i; j;

	for (i=0; i<pory1->HighPower; i++){
		for (j=0; j<pory2->HighPower; j++){
			polyProduct->coefficient_array[i+j] = poly1->coefficient_array[i] * poly2->coefficient_array[j];
		}
	}
}


