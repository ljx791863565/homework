#include <iostream>

class CParent{

	public:

		CParent(){}

		virtual ~CParent(){}

			public:

				virtual void Print()

				{

					std::cout<< "1,";

				}

		};

		class CSon : public CParent{

			public:

				CSon(){}

				virtual ~CSon(){}

			public:

				void print()

				{

					std::cout<<"2,";

				}

		};

		void Test1(CParent& oParent)

		{

			oParent.Print();

		}

		void Test2(CParent oParent)

		{

			oParent.Print();

		}



		int main()

		{

			CSon *p = new CSon();

			Test1(*p);

			Test1(*p);

			Test2(*p);

			delete p;
			CParent *p2 = new CParent();
			Test1(*p2);
			Test2(*p2);
		}
