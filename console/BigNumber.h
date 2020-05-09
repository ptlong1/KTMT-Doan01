#pragma once
#include <iostream>
#include <string>
using namespace std;

class BigNumber
{
private:
	string m_num;
public:
	BigNumber();
	BigNumber(string);
	BigNumber(const BigNumber& other);
	~BigNumber();

	void fixNum();
	void print();
	string getNum();
	BigNumber operator + (const BigNumber& other);
	void multi2();//nhan voi 2
	void divide2();
	void multiPow2(int m);//nhan voi 2 mu m

	BigNumber& operator = (const BigNumber& other);
	BigNumber& operator = (const int& x);//gan bang 1 so nguyen

};
