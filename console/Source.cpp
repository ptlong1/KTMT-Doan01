#include "QInt.h"

using namespace std;

int main()
{
	QInt A("10", 10), B("-7", 10);
	//QInt tmp("1", 10);
	//cout << D.toBin() << '\n' << A.toBin() << '\n' << (~A + tmp).toBin();
	//A.PrintQInt(cout, 10);
	//cout << A.toDec() << '\n';

	cout << (A).toDec() << '\n' << (B).toDec() << '\n';

	//A.PrintQInt(cout, 10);
	cout << "Cong 2 so: " << (A + B).toBin() << '\n';
	cout << "Tru 2 so: " << ((A - B)).toDec() << '\n';

	cout << "Nhan 2 so: " << (A * B).toDec() << '\n';
	//cout << (A + A).toBin() << '\n';
	cout << "Chia 2 so: " << (A / B).first.toDec() << '\n' << (A / B).second.toDec() << '\n';
	
	/*
	QInt C = A << 1;
	cout << "Shift left: " << C.toBin() << '\n';
	C = A >> 1;
	cout << "Shift right: " << C.toBin() << '\n';
	*/
}