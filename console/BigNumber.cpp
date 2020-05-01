#include "BigNumber.h"

BigNumber::BigNumber()
{
	m_size = 50;
	m_num.resize(m_size, '0');
}

BigNumber::BigNumber(string str)
{
	m_size = 50;
	m_num.resize(m_size);

	for (int i = 0; i <= str.length(); i++) {
		m_num[m_size + i - str.length() - 1] = str[i];
	}
}

BigNumber::BigNumber(const BigNumber& other)
{
	m_size = 50;
	m_num.resize(m_size, '0');

	for (int i = 0; i <= m_size; i++) {
		m_num[i] = other.m_num[i];
	}
}

BigNumber::~BigNumber()
{

}

void BigNumber::print()
{
	cout << m_num << endl;
}

string BigNumber::getNum()
{
	return m_num;
}

BigNumber BigNumber::operator + (const BigNumber& other) {
	bool carry = false;
	BigNumber rs;
	for (int i = m_size - 1; i >= 0; i--) {
		char temp = this->m_num[i] + other.m_num[i] - '0';
		if (carry) {
			temp++;
			carry = false;
		}

		if (temp > '9') {
			temp -= 10;
			carry = true;
		}

		rs.m_num[i] = temp;
	}

	return rs;
}

void BigNumber::multi2()
{
	*this = *this + *this;
}

void BigNumber::multiPow2(int m)
{
	for (int i = 1; i <= m; i++) {
		this->multi2();
	}
}

bool BigNumber::operator < (const BigNumber& other)
{
	for (int i = 0; i < m_size; i++) {
		if (this->m_num[i] < other.m_num[i])
			return true;
	}
	return false;
}

BigNumber& BigNumber::operator=(const BigNumber& other)
{
	if (this != &other) {
		for (int i = 1; i < m_size; i++) {
			this->m_num[i] = other.m_num[i];
		}
	}
	return *this;
}

BigNumber& BigNumber::operator=(const int& x)
{
	unsigned int temp = x;
	for (int i = m_size - 1; i >= 0; i--) {
		m_num[i] = (temp % 10) + '0';
		temp /= 10;
	}
	return *this;
}
