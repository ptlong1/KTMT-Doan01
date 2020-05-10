#pragma once
#include <iostream>
#include <string>
#include "QInt.h"
#include "BigNumber.h"
#include "BitHandle.h"

using namespace std;
class QFloat
{
private:
	int m_arr[4];
	

	QFloat getZero();
	QFloat getInf();
	QFloat getNaN();

	static const int SIGNIFICANT_SIZE = 112;
	static const int EXPONENT_BIAS_VALUE = 16383; // = 2^14 -1
	static const int EXPONENT_SIZE = 15;
	static const int SIZE = 128;
	
	//Hàm cho scanf
	vector<int> convert2vt_bin(string str, int type);
	vector<int> str_dec2bin(string str);
	vector<int> str_hex2bin(string str);
	vector<int> str_bin2bin(string str);
	void str_devide2(string& str);
	void str_multi2(string& str);
	//Hàm cho operator*
	vector<int> get_exponent() const;
	vector<int> get_snfcant() const;
	vector<int> get_full_snfcant() const;
	bool check_all_bit0(vector<int>) const;
	bool check_all_bit1(vector<int>) const;
	void set_all_bit0(vector<int>&);
	void set_all_bit1(vector<int>&);
	vector<int> add_exponent(vector<int>, vector<int>, int carry);
	vector<int> multi_snfcant(vector<int>, vector<int>, int& carry);
	void handle_multi_special_case(int flag, vector<int>& exp, vector<int>& snfc);
	/*-------------------------------------------------------------*/
	string divideSignificand(string snf1, string snf2);

public:
	QFloat();
	QFloat(vector<int> vtBin);
	QFloat(string str, int base);
	~QFloat();

	bool isZero() const;//kiem tra co phai la so 0 hay k?
	bool isInf() const;//Kiem tra co phai la so vo cung lon hay khong?
	bool isNaN() const;//Kiem tra co la truong loi hay?
	bool isDenorm() const; //Kiem tra so khong chuan

	void setZero();
	void setNaN();
	void setInf(int sign);

	int getSign() const;//0: duong, 1: am
	int getExp();
	string getSignificand();
	int checkExponent();//kiem tra phan mu co bang 00...00 hay 11...11 khong

	
	string toDec();
	string toBin();

	void ScanQFloat(istream& f, int base);
	void PrintQFloat(ostream& f, int base);
	void PrintQFloat1(ostream& f, int base);

	QFloat operator + ( QFloat other);
	QFloat operator - ( QFloat other);
	QFloat operator * (const QFloat& other);
	QFloat operator / (const QFloat& other);

	QFloat& operator = (const QFloat& other);



	int getBit(int i) const;
	void setbit1(int i);
	void setbit0(int i);

	QFloat DoiDau();
	int operator [] (int i);
	QFloat operator >> (int x);
	QFloat operator << (int x);
	bool operator >= (QFloat other);
	QFloat CongPhanMu(int x);
	void GanDauvaMu(QFloat x);
};

