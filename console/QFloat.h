#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;


class QFloat
{
private:
	static const int SIGNIFICANT_SIZE = 112;
	static const int EXPONENT_SIZE = 15;
	static const int SIZE = 128;
	int m_arr[4];
	vector<int> convert2vt_bin(string str, int type);
	vector<int> str_dec2bin(string str);
	vector<int> str_hex2bin(string str);
	vector<int> str_bin2bin(string str);
	void str_devide2(string& str);
	void str_multi2(string& str);
	vector<int> add_exponent(vector<int>, vector<int>);
	vector<int> get_exponent() const;
	vector<int> get_snfcant() const;
	vector<int> multi_snfcant(vector<int>, vector<int>);
	bool check_all_bit0(vector<int>);
	void set_all_bit0(vector<int>&);
	void handle_multi_special_case(int flag, vector<int>& exp, vector<int>& snfc);
public:
	QFloat();
	QFloat(string str, int base);
	QFloat(vector<int> vtBin);
	~QFloat();

	bool isZero() const;
	string toDec();
	string toBin();

	void ScanQFloat(istream& f, int base);
	void PrintQFloat(ostream& f, int base);

	QFloat operator + (const QFloat& other);
	QFloat operator - (const QFloat& other);
	QFloat operator * (const QFloat& other);
	
	pair<QFloat, QFloat> operator / (const QFloat& other);
	int& operator[](int i);

	int getBit(int i) const;
	void setbit1(int i);
	void setbit0(int i);
	void setbit(int i, int bit);
	QFloat DoiDau();
};

