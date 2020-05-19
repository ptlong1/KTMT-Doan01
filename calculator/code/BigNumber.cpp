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

/*Xóa các số 0 vô nghĩa ở đầu*/
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

/*In ra màn hình console*/
void BigNumber::print()
{
	this->fixNum();
	cout << m_num << endl;
}

/*Lấy số bigNumber đang được lưu*/
string BigNumber::getNum()
{
	this->fixNum();
	if (m_num == "") return "0";
	return m_num;
}

/*Toán tử cộng*/
BigNumber BigNumber::operator+(const BigNumber & other)
{
	string str1 = m_num;
	string str2 = other.m_num;

	if (str1.length() > str2.length())
		swap(str1, str2);

	string str = "";// Lưu kết quả

	int n1 = str1.length(), n2 = str2.length();

	// Đảo chuổi
	reverse(str1.begin(), str1.end());
	reverse(str2.begin(), str2.end());

	int carry = 0;
	for (int i = 0; i < n1; i++)
	{
		// cộng 2 chữ số tương ứng và cộng thêm nhớ (nếu có)
		int sum = ((str1[i] - '0') + (str2[i] - '0') + carry);
		str.push_back(sum % 10 + '0');

		carry = sum / 10; //lưu số nhớ
	}

	//cộng tiếp vào phần dư ra 
	for (int i = n1; i < n2; i++)
	{
		int sum = ((str2[i] - '0') + carry);
		str.push_back(sum % 10 + '0');
		carry = sum / 10;
	}

	// thêm số 1 nếu như còn nhớ ở chữ số cuối cùng 
	if (carry)
		str.push_back(carry + '0');

	//đảo lại chuỗi
	reverse(str.begin(), str.end());

	return BigNumber(str);
}

//Nhân một số nguyên với hai
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

//Nhân một số thực với hai
void BigNumber::divide2_float()
{
	string x = m_num;
	int pos_point = x.find('.');
	if (pos_point == -1) {
		pos_point = x.size();
	}
	string a = x.substr(0, pos_point);
	string b = (pos_point == x.size()) ? "0" : x.substr(pos_point + 1);

	bool check_odd = false;
	if ((a[a.size() - 1] - '0') % 2 == 1) {
		check_odd = true;
		a[a.size() - 1] -= 1;
		b = '1' + b;
	}

	BigNumber m(a);
	m.divide2(); a = m.getNum();

	if ((b[b.size() - 1] - '0') % 2 == 1) {
		b += '0';
	}

	int i = 0;
	for (; i < b.size(); i++) {
		if (b[i] != '0')
			break;
	}

	if (i != b.size()) {
		string temp = b.substr(i);
		BigNumber n(temp);
		n.divide2();
		if (temp[0] == '1' && !check_odd) i++;
		b = string(i, '0') + n.getNum();
	}
	
	i = b.size() - 1;
	for (; i >= 0; i--)
		if (b[i] != '0')
			break;
	b.erase(i+1);

	m_num = (i == -1) ? a : a + '.' + b;
}

//Nhân một số với 2 mũ m (m có thể âm hoặc dương)
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
		for (int i = 1; i <= m; i++)
			this->divide2_float();
	}
}

//toán tử gán bằng một số BigNumber khác
BigNumber& BigNumber::operator=(const BigNumber & other)
{
	if (this != &other) {
		m_num.clear();
		m_num = other.m_num;
	}
	return *this;
}

//Toán tử gán bằng 1 số nguyên int
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
