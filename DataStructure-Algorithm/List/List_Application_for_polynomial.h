#ifndef _POLYNPOMIAL_H_
#define _POLYNPOMIAL_H_
struct Node {
	int coefficient;
	int exponent;
	struct Node *next;
};

typedef struct Node* Polynomial;

Polynomial initPolynomial(Polynomial p);
void Insert(int coeff, int expo, Polynomial p);
void PrintPolynomial(Polynomial p);
void AddPolynomial(Polynomial p1, Polynomial p2, Polynomial psum);
Polynomial Find(int expo, Polynomial p);
void Delete(int expo, Polynomial p);
void DeleteRepeat(Polynomial p);
Polynomial FindPrevious(int expo, Polynomial p);

#endif
