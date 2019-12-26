#include <stdio.h>
#include <stdlib.h>
#include "List_Application_for_polynomial.h"
int main()
{
	Polynomial p1 = NULL;
	p1 = initPolynomial(p1);
	if (p1 == NULL){
		printf("init poly fail\n");
	}
	Polynomial p2 = NULL;
	p2 = initPolynomial(p2);
	if (p2 == NULL){
		printf("init poly fail\n");
	}
	
	Insert(5, 1024, p1);
	Insert(34, 563, p1);
	Insert(18, 2, p1);
	Insert(129, 0, p1);
	
	Insert(69, 963, p2);
	Insert(12, 961, p2);
	Insert(256, 8, p2);
	Insert(1012, 0, p2);

	PrintPolynomial(p1);
	PrintPolynomial(p2);
	
	Polynomial psum = NULL;
	psum = initPolynomial(psum);
	AddPolynomial(p1, p2, psum);
	PrintPolynomial(psum);
	return 0;

}
