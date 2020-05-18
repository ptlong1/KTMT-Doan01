#include "pch.h"
#include "BigNumber.h"



BigNumber::BigNumber()
{
	m_num.resize(50, '0');
}

BigNumber::BigNumber(string str)
{
	m_num = str;
}

BigNumber::BigNumber(const BigNumber & other)
{
	m_num = other.m_num;
}

BigNumber::~BigNumber()
{
}

void BigNumber::fixNum()
{
	int i = 0;
	int size = m_num.size();
	for (; i < size; i++) {
		if (m_num[i] != '0')
			break;
	}
	if (m_num[i] == '.')
		i--;
	m_num.erase(0, i);
}

void BigNumber::print()
{
	this->fixNum();
	cout << m_num << endl;
}

string BigNumber::getNum()
{
	this->fixNum();
	if (m_num == "") return "0";
	return m_num;
}

BigNumber BigNumber::operator+(const BigNumber & other)
{
	string str1 = m_num;
	string str2 = other.m_num;

	if (str1.length() > str2.length())
		swap(str1, str2);

	// Take an empty string for storing result 
	string str = "";

	// Calculate length of both string 
	int n1 = str1.length(), n2 = str2.length();

	// Reverse both of strings 
	reverse(str1.begin(), str1.end());
	reverse(str2.begin(), str2.end());

	int carry = 0;
	for (int i = 0; i < n1; i++)
	{
		// Do school mathematics, compute sum of 
		// current digits and carry 
		int sum = ((str1[i] - '0') + (str2[i] - '0') + carry);
		str.push_back(sum % 10 + '0');

		// Calculate carry for next step 
		carry = sum / 10;
	}

	// Add remaining digits of larger number 
	for (int i = n1; i < n2; i++)
	{
		int sum = ((str2[i] - '0') + carry);
		str.push_back(sum % 10 + '0');
		carry = sum / 10;
	}

	// Add remaining carry 
	if (carry)
		str.push_back(carry + '0');

	// reverse resultant string 
	reverse(str.begin(), str.end());

	return BigNumber(str);
}

void BigNumber::multi2()
{
	this->fixNum();
	bool carry = false;
	int size = m_num.size();

	for (int i = size - 1; i >= 0; i--) {
		char x = m_num[i] * 2 - '0';
		if (carry) {
			x++;
			carry = false;
		}

		if (x > '9') {
			x -= 10;
			carry = true;
		}

		m_num[i] = x;
	}

	if (carry)
		m_num = "1" + m_num;

}

void BigNumber::divide2()
{
	this->fixNum();

	string ans;
	int quotient = 0;
	int remainder = 0;
	int divident = 0;

	int i = 0;
	if (m_num[i] == '1') {
		remainder = 1;
		i++;
	}

	int max = m_num.size();//(i + 50 > m_num.size()) ? m_num.size() : i + 50;
	for (int j = i; j < max; j++) {//vi co nhieu nhat la 50 so khac 0 tinh tu trai qua
		divident = (remainder * 10 + m_num[j] - '0');
		ans.push_back(divident / 2 + '0');
		remainder = divident % 2;
	}
	m_num = ans + m_num.substr(max);
}

void BigNumber::multiPow2(int m)
{
	this->fixNum();
	if (m >= 0) {
		for (int i = 1; i <= m; i++) {
			this->multi2();
		}
	}
	else {
		m = -m;
		/*int number_digit = int(log10(2)*m) + 1 - m_num.size();
		m_num += '0';
		if (number_digit > 0) {
			m_num += string(number_digit, '0');
		}*/
		int num0 = 5000;
		m_num += string(num0, '0');
		for (int i = 1; i <= m; i++) {
			this->divide2();
		}

		if (m_num.size() <= num0) {
			m_num = string(num0 - m_num.size() + 2, '0') + m_num;
			m_num[1] = '.';
		}
		else {
			m_num.insert(m_num.size() - num0, ".");
		}
		int i = m_num.size() - 1;
		while (i > 0) {
			if (m_num[i] != '0' && m_num[i] != '.') {
				break;
			}
			i--;
		}
		m_num.erase(i + 1, m_num.size() - i - 1);
	}
}

BigNumber& BigNumber::operator=(const BigNumber & other)
{
	if (this != &other) {
		m_num.clear();
		m_num = other.m_num;
	}
	return *this;
}

BigNumber & BigNumber::operator=(const int & x)
{
	unsigned int temp = x;
	m_num.resize(50, '0');
	for (int i = m_num.size() - 1; i >= 0; i--) {
		m_num[i] = (temp % 10) + '0';
		temp /= 10;
	}
	return *this;
}
