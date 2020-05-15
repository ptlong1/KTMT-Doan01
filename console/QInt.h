#pragma once
#include <iostream>
#include "BitHandle.h"
#include "BigNumber.h"
#include <string>
#include <stdio.h>
#include <vector>
#include <map>

using namespace std;

const int N = 4, M = 32;

class QInt {
private:
	int m_arr[4];
public:
	QInt();
	QInt(string str, int base);

	/* 3 ong lon */

	string toString(int base);
	void ScanQInt(istream& f, int base);
	void PrintQInt(ostream& f, int base); //xuat ra ket qua he co so option(option : 2, 10, 16,...)

	string toDec();
	string toBin();
	string toHex();
	

	QInt operator + (const QInt& other);
	QInt operator - (const QInt& other);
	QInt operator * (const QInt& other);
	pair<QInt, QInt> operator / (const QInt& other);

	bool operator > (QInt other);
	bool operator >= (QInt other);
	bool operator < (QInt other);
	bool operator <= (QInt other);
	bool operator != (QInt other);
	bool operator == (QInt other);

	QInt& operator = (const QInt& other);
	QInt& operator = (string);

	QInt operator & (const QInt& other);
	QInt operator | (const QInt& other);
	QInt operator ^ (const QInt& other);
	QInt operator ~ ();

	QInt operator >> (int x);
	QInt operator << (int x);
	QInt rol (int x);
	QInt ror (int x);

	//=========================================================
	///Ham them
	QInt DoiDau();
	int operator [] (int i);

	//lay bit thu i, tinh tu phai qua
	int getBit(int i) const;
	void setbit1(int i);
	void setbit0(int i);
	QInt getTowComplement();

	//convert
	vector<int> convert2vt_bin(string str, int base);
	vector<int> str_hex2bin(string str);
	vector<int> str_bin2bin(string str);
	string devide_strBy2(string str);
	vector<int>	str_dec2bin(string str);


	//==========================================================
};
