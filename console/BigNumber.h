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
	void multi2(); //nhân 1 số nguyên với 2
	void divide2(); //chia 1 số nguyên cho 2
	void divide2_float(); //chia 1 số thực cho 2
	void multiPow2(int m); //nhân với 2 mũ m (m có thể âm hoặc dương)

	BigNumber& operator = (const BigNumber& other);
	BigNumber& operator = (const int& x);//gan bang 1 so nguyen

};
