
using namespace std;
class Comples
{
	double re;
	double im;
public:
	Comples(double r, double i)
	{
		re = r;
		im = i; 
	}
	Comples(double r)
	{
		re = r;
		im = 0;
	}
	Comples()
	{
		re = im = 0;
	}
	
	
	friend Comples operator+(Comples &a, Comples &b);
	friend Comples operator-(Comples &a, Comples &b);
	friend Comples operator-(Comples &a);
	friend Comples operator*(Comples &a, Comples &b);
	friend Comples operator/(Comples &a, Comples &b);

	friend Comples operator==(Comples &a, Comples &b);
	friend Comples operator!=(Comples &a, Comples &b); 

	void show(); 
};

