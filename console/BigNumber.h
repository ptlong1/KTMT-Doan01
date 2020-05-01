#pragma once
#include <iostream>
#include <stdio.h>
#include <string>


using namespace std;

//so trong he thap phan
class BigNumber
{
private:
	string m_num;
	int m_size; // so chu so thap phan
public:
	BigNumber();
	BigNumber(string);
	BigNumber(const BigNumber& other);
	~BigNumber();

	void print();
	string getNum();
	BigNumber operator + (const BigNumber& other);
	void multi2();//nhan voi 2
	void multiPow2(int m);//nhan voi 2 mu m

	bool operator < (const BigNumber& other);
	BigNumber& operator = (const BigNumber& other);
	BigNumber& operator = (const int& x);//gan bang 1 so nguyen
};

