#include "list.h"
#include <stdio.h>
#define Max(x,y) ((x) > (y) ? (x) : (y))

typedef struct tagPOLYNOMIAL
{
	CSList<int> Coeff;
	int HighPower; //最高阶指数值
}*Polynomial;

static void AddPolynomial(
		Polynomial polysum, 
		const Polynomial poly1, 
		const Polynomial poly2)
{
	int i;
	int sum;
	int tmp1;
	int tmp2;

	polysum->HighPower = Max(poly1->HighPower, poly2->HighPower);

	for (i=1; i<=polysum->HighPower +1; ++i){
		tmp1 = poly1->Coeff.GetAt(i);
		tmp2 = poly2->Coeff.GetAt(i);
		sum = tmp1 + tmp2;
		polysum->Coeff.AddTail(sum);
	}
}

static void SubPolynomial(
		Polynomial polysub, 
		const Polynomial poly1, 
		const Polynomial poly2)
{
	int i;
	int sub;
	int tmp1;
	int tmp2;

	polysub->HighPower = Max(poly1->HighPower, poly2->HighPower);

	for (i=1; i<=polysub->HighPower +1; ++i){
		tmp1 = poly1->Coeff.GetAt(i);
		tmp2 = (-1)*poly2->Coeff.GetAt(i);
		sub = tmp1 + tmp2;
		polysub->Coeff.AddTail(sub);
	}
}

static void MulPolynomial(
		Polynomial polymul,
		const Polynomial poly1,
		const Polynomial poly2)
{
	int i, j;
	int tmp, tmp1, tmp2;

	polymul->HighPower = poly1->HighPower + poly2->HighPower;
	
	for (i=0; i<=polymul->HighPower; ++i){
		polymul->Coeff.AddTail(0);
	}

	for (i=0; i<=poly1->HighPower; i++){
		tmp1 = poly1->Coeff.GetAt(i+1);
	//	printf("tmp1 = %d i = %d j = %d\n", tmp1, i, j);
		for (j=0; j<=poly2->HighPower; j++){
			tmp = polymul->Coeff.GetAt(i+j+1);
			tmp2 = poly2->Coeff.GetAt(j+1);
	//		printf("tmp2 = %d i = %d j = %d\n", tmp2, i, j);
			tmp += tmp1 * tmp2;
	//		printf("tmp = %d i = %d j = %d\n\n", tmp, i, j);
			polymul->Coeff.SetAt(i+j+1, tmp);
		}
	}
}

static void PrintPoly(const Polynomial poly)
{
	int i;

	for (i=poly->HighPower; i>0; --i){
		printf("%dX^%d + ",poly->Coeff.GetAt(i+1), i);
	}
	printf("%d\n", poly->Coeff.GetHead());
}

int main()
{
	Polynomial poly1 = NULL;
	Polynomial poly2 = NULL;
	Polynomial polyresult = NULL;

	poly1 = new (struct tagPOLYNOMIAL);
	if (NULL == poly1){
		goto Exit0;
	}

	poly2 = new (struct tagPOLYNOMIAL);
	if (NULL == poly2){
		goto Exit0;
	}

	polyresult = new (struct tagPOLYNOMIAL);
	if (NULL == polyresult){
		goto Exit0;
	}

	//P1(X) = X^2 + 2X + 3
	poly1->HighPower = 2;
	poly1->Coeff.AddHead(0); 
	poly1->Coeff.AddHead(1);
	poly1->Coeff.AddHead(2);
	poly1->Coeff.AddHead(3);

	//P2(X) = 3X^3 + 2X^2 + 10X + 6
	poly2->HighPower = 3;
	poly2->Coeff.AddHead(3);
	poly2->Coeff.AddHead(2);
	poly2->Coeff.AddHead(10);
	poly2->Coeff.AddHead(6);
	
	printf("P1 = ");
	PrintPoly(poly1);
	printf("P2 = ");
	PrintPoly(poly2);
/*
	//P1(X) + P2(X) = 3X^3 + 3X^2 + 12X + 9
	AddPolynomial(polyresult, poly1, poly2);
	printf("p1 + p2 = ");
	PrintPoly(polyresult);

	polyresult->Coeff.RemoveAll();

	//P1(X) - P2(X) = -3X^3 - X^2 + -8X - 3
	SubPolynomial(polyresult, poly1, poly2);
	printf("p1 - p2 = ");
	PrintPoly(polyresult);
*/
	//P1(X) * P2(X) = 3X^5 + 6X^4 + 19X^3 + 26X^2 + 42X + 18
	MulPolynomial(polyresult, poly1, poly2);
	printf("p1 * p2 = ");
	PrintPoly(polyresult);

Exit0:
	if (poly1){
		delete poly1;
		poly1 = NULL;
	}
	if (poly2){
		delete poly2;
		poly2 = NULL;
	}
	if (polyresult){
		delete polyresult;
		polyresult = NULL;
	}
}
