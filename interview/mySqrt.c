#include <stdio.h>

double mySqrt(int a, double e)
{
	if (a < 0){
		return a;
	}
	double mid, last;
	double low, up;
	low = 0, up = a;
	mid = (low + up) / 2;
	do {
		if (mid * mid > a)
			up = mid;
		else
			low = mid;
		last = mid;
		mid = (up + low) / 2;
	}while (abs(mid-last) > e);

	return mid;
}

double newtonSqrt(int n, double e)
{
	if (n < 0){
		return n;
	}
	double val = n;
	double last;
	do{
		last = val;
		val = (val + n/val) / 2;
	} while (abs(val - last) > e);
	return val;
}
int main()
{
	double a = 4;
	double num = newtonSqrt(a, 0.001);

	printf("aqrt(%f) = %.3f\n", a, num);
}
