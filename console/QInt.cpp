
#include "QInt.h"

QInt::QInt()
{
	for (int i = 0; i < 4; i++)
		m_arr[i] = 0;
}

QInt::QInt(string str, int base)
{
	/*
PhƯơng thức khởi tạo từ str theo base bất kì (2,10,16)
*/
	for (int i = 0; i < 4; i++)
		m_arr[i] = 0;
	vector<int> vtBin = convert2vt_bin(str, base);
	for (int i = 0; i < vtBin.size(); i++) {
		m_arr[3 - i / 32] = m_arr[3 - i / 32] | (vtBin[i]) << (i % 32);			//getBit và setBit
	}

}


void QInt::ScanQInt(istream& f, int base)
{	/*
	Đọc một string từ f theo base, chuyển thành từng bit và lưu vào this
	*/
	string str;
	//getline(f, str, ' ');
	f >> str;
	QInt tmp(str, base);
	*this = tmp;
}

void QInt::PrintQInt(ostream& f, int base)
{
	/*
In ra f theo base
*/
	string ans;
	if (base == 2) ans = this->toBin();
	else if (base == 10) ans = this->toDec();
	else if (base == 16) ans = this->toHex();
	if (base == 10)
		f << ans << endl;
	else {
		int n = ans.size();
		for (int i = 0; i < n; i++) {
			if (i!=0 && (i % (n/8)) == 0) cout << " ";
			cout << ans[i];
		}
	}

}

string QInt::toString(int base) {
	/*
	Chuyển QInt về base nào đó, trả về string
	*/
	if (base == 2) return this->toBin();
	else if (base == 10) return this->toDec();
	else return this->toHex();
}
string QInt::toDec()
{
	string rs(50, '0');
	char signThis = this->getBit(127);
	QInt temp = *this;
	if (signThis == 1)
		temp = temp.getTowComplement();

	BigNumber numDec;
	numDec = 0;

	for (int i = 0; i <= 3; i++) {
		BigNumber k;
		k = temp.m_arr[i];
		k.multiPow2(32 * (3 - i));
		numDec = numDec + k;
	}

	rs = numDec.getNum();

	if (signThis == 1) {
		rs = '-' + rs;
	}

	if (rs.empty()) rs.push_back('0');
	return rs;
}

string QInt::toBin()
{
	string rs(128, '0');

	for (int i = 0; i < 128; i++) {
		rs[i] = getBit(128 - i - 1) + '0';
	}

	int i = 0;
	for (; i < rs.length(); i++) {
		if (rs[i] != '0')
			break;
	}
	rs.erase(0, i);
	if (rs.empty()) rs.push_back('0');
	return rs;
}

string QInt::toHex()
{
	string rs(32, '0');

	for (int i = 0; i < 32; i++) {
		char x = 0;
		for (int j = 3; j >= 0; j--) {
			x = 2 * x + getBit(i * 4 + j);
		}
		switch (x)
		{
		case 10: x = 'A'; break;
		case 11: x = 'B'; break;
		case 12: x = 'C'; break;
		case 13: x = 'D'; break;
		case 14: x = 'E'; break;
		case 15: x = 'F'; break;
		default:
			x += '0';
			break;
		}
		rs[31 - i] = x;
	}
	int i = 0;
	for (; i < rs.length(); i++) {
		if (rs[i] != '0')
			break;
	}
	rs.erase(0, i);
	if (rs.empty()) rs.push_back('0');
	return rs;
}

QInt QInt::operator+(const QInt& other)
// cong tung bit lai voi nhau
{
	QInt ans;
	int nho = 0;
	for (int i = 3; i >= 0; --i)
	{
		for (int j = 31; j >= 0; --j)
		{
			//dung i, j xac dinh vi tri bit
			nho += Getbit(m_arr[i], M - j - 1) + Getbit(other.m_arr[i], M - j - 1);
			ans.m_arr[i] += (nho % 2) * (1 << (M - j - 1));
			nho = nho / 2;
		}
	}
	return ans;
}

QInt QInt::operator-(const QInt& other)
{
	QInt tmp = other;
	// tru = cong voi doi dau
	//cout << tmp.toBin() << '\n';
	return (*this + tmp.DoiDau());
}

QInt QInt::operator*(const QInt& other)
{
	//thuat toan booth
	QInt A, Q, M;
	int Q0, Q1, cnt = 128;
	Q = *this;
	M = other;
	Q1 = Q0 = 0;
	while (cnt--)
	{
		//cout << "***\n";
		//cout << A.toBin() << '\n' << Q.toBin() << '\n' << M.toBin() << '\n';
		Q0 = Q[0];
		//cout << "Q0 Q1 = " << Q0 << ' ' << Q1 << '\n';
		if (Q1 != Q0)
		{
			if (Q1 == 1)
				A = A + M;
			if (Q1 == 0)
				A = A - M;
			//cout << "A-+: " << A.toBin() << '\n';
		}
		Q1 = Q0;
		Q = Q >> 1;
		Q.m_arr[0] += A[0] * (1 << 31);
		A = A >> 1;
		//cout << "A >> 1: " << A.toBin() << '\n';
		//cout << "A126: " << A[126] << '\n';
		A.m_arr[0] += A[126] * (1 << 31);
		//cout << "A: " << A.toBin() << '\n';
	}
	//chi lay 128 bit cuoi
	return Q;
}

pair<QInt, QInt> QInt::operator/(const QInt& other)
//return {thuong, so du} 
{
	QInt A, Q, M, zero;
	Q = *this;
	M = other;
	//cout << Q.toBin() << '\n' << M.toBin() << '\n';
	int dau = (Q[127] != M[127]);
	if (Q[127] == 1) Q = Q.DoiDau();
	if (M[127] == 1) M = M.DoiDau();
	int cnt = 128;
	while (cnt--)
	{
		//cout << "======================\n";
		//cout << "A: " << A.toBin() << '\n';
		//cout << "Q: " << Q.toBin() << '\n';
		A = A << 1;
		A.m_arr[3] += Q[127] * 1;
		Q = Q << 1;
		//cout << "A1: " << A.toBin() << '\n';
		//cout << "Q1: " << Q.toBin() << '\n';
		A = A - M;
		if (A < zero)
		{
			A = A + M;
		}
		else Batbit(Q.m_arr[3], 0);
	}
	// -10/-7 = 1 du -3
	if (dau) Q = Q.DoiDau();
	//cout << A.toDec() << '\n';
	if ((Q * other + A) != (*this)) A = A.DoiDau();
	//cout << "Q: " << Q.toDec() << '\n';
	//cout << "A: " << A.toDec() << '\n';
	//cout << "other: " << other.toDec() << '\n';
	//cout << (Q * other + A).toDec() << '\n';
	//cout << (*this).toDec() << '\n';
	//cout << ((Q * other + A) != (*this)) << '\n';
	return { Q, A };
}

bool QInt::operator > (QInt other)
{
	int signThis = this->getBit(127);
	int signOther = other.getBit(127);
	if (signThis == signOther)
	{
		for (int i = 1; i < 128; i++) {
			if (this->getBit(127 - i) > other.getBit(127 - i))
				return true;
			if (this->getBit(127 - i) < other.getBit(127 - i))
				return false;
		}
	}
	else
	{
		if (signThis == 0)
			return true;
	}
	return false;
}

bool QInt::operator >= (QInt other)
{
	return !(*this < other);
}

bool QInt::operator < (QInt other)
{
	int signThis = this->getBit(127);
	int signOther = other.getBit(127);
	if (signThis == signOther)
	{
		for (int i = 1; i < 128; i++) {
			if (this->getBit(127 - i) < other.getBit(127 - i))
				return true;
			if (this->getBit(127 - i) > other.getBit(127 - i))
				return false;
		}
	}
	else
	{
		if (signThis == 1)
			return true;
	}
	return false;
}

bool QInt::operator <= (QInt other)
{
	return !(*this > other);
}

bool QInt::operator != (QInt other)
{
	for (int i = 127; i >= 0; i--) {
		if (this->getBit(i) != other.getBit(i))
			return true;
	}
	return false;
}

bool QInt::operator == (QInt other)
{
	return !(*this != other);
}

QInt& QInt::operator = (const QInt& other)
{
	if (this != &other) {
		for (int i = 0; i < 4; i++)
			m_arr[i] = other.m_arr[i];
	}
	return *this;
}

QInt& QInt::operator = (string str)
{
	memset(m_arr, 0, 4);
	int lenStr = str.length();

	for (int i = lenStr - 1; i >= 0; i--) {
		if (str[i] == '1') {
			setbit1(lenStr - 1 - i);
		}
		else
			setbit0(lenStr - 1 - i);
	}

	return *this;
}

QInt QInt::operator&(const QInt& other)
{
	/*
	Operator & cho QInt
	*/
	string str_bin;
	for (int i = 127; i >= 0; i--) {
		str_bin.push_back((this->getBit(i) & other.getBit(i)) + '0');
	}
	QInt ans(str_bin, 2);
	return ans;
}

QInt QInt::operator|(const QInt& other)
{
	/*
	Operator | cho QInt
	*/
	string str_bin;
	for (int i = 127; i >= 0; i--) {
		str_bin.push_back((this->getBit(i) | other.getBit(i)) + '0');
	}
	QInt ans(str_bin, 2);
	return ans;
}

QInt QInt::operator^(const QInt& other)
{
	/*
	Operator ^ cho QInt
	*/
	string str_bin;
	for (int i = 127; i >= 0; i--) {
		str_bin.push_back((this->getBit(i) ^ other.getBit(i)) + '0');
	}
	QInt ans(str_bin, 2);
	return ans;
}

QInt QInt::operator~()
{
	/*
	Operator ~ cho QInt

	string str_bin;
	for (int i = 127; i >= 0; i--) {
		str_bin.push_back(!(this->getBit(i)));
	}
	QInt ans(str_bin, 2);
	for (int i = 0; i < 4; ++i) cout << ans.m_arr[i] << ' ';
	return ans;
	*/
	QInt ans = *this;
	for (int i = 0; i < 4; ++i) ans.m_arr[i] = ~ans.m_arr[i];
	return ans;
}

QInt QInt::operator>>(int x)
{
	QInt ans;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 32; ++j)
		{
			// lay bit o vi tri lien sau no
			// vi tri cua bit hien tai la i*32 + j
			int vt = M * (N - i - 1) + (M - j - 1);
			ans.m_arr[i] += (1 << (M - j - 1)) * (*this)[vt + x];
		}
	return ans;
}

QInt QInt::operator<<(int x)
{
	QInt ans;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 32; ++j)
		{
			// lay bit o vi tri lien truoc no
			// vi tri cua bit hien tai la i*32 + j
			int vt = M * (N - i - 1) + (M - j - 1);
			ans.m_arr[i] += (*this)[vt - x] * (1 << (M - j - 1));
		}
	return ans;
}

QInt QInt::rol(int x)
{
	QInt ans = *this;
	ans = ans << x; // dich trai
	// fullfill x bit dau tien
	for (int i = 1; i <= x; ++i)
	{
		int vti, vtj;
		vti = (x - i) / 32;
		vtj = (x - i) % 32;
		ans.m_arr[vti] = (*this)[128 - i] * (1 << vtj);
	}
	return ans;
}

QInt QInt::ror(int x)
{
	x = x % 128;
	QInt ans = *this >> x;
	//fullfill x bit cuoi cung
	for (int i = 0; i < x; i++) {
		int temp = getBit(i);
		if (temp == 1)
			ans.setbit1(127 - x + 1 + i);
		else
			ans.setbit0(127 - x + 1 + i);
	}

	return ans;
}

QInt QInt::DoiDau()
{
	//doi dau gia tri
	QInt ans = *this;
	QInt tmp("1", 10);
	ans = ~ans + tmp;
	//cout << ans.m_arr[3] << '\n';
	return ans;
}

int QInt::operator[](int i)
// gia tri bit thu i
{
	//127 vti = 3, vtj = 31, 0, 0, 
	if (i > 127 || i < 0) return 0;
	int vti, vtj;
	vti = i / 32;
	vtj = i % 32;
	return Getbit(this->m_arr[N - 1 - vti], vtj);
}

int QInt::getBit(int i) const
{
	i = i % 128;
	return ((m_arr[3 - i / 32] >> (i % 32)) & 1);
}

void QInt::setbit1(int i)
{
	m_arr[3 - i / 32] = m_arr[3 - i / 32] | (1 << (i % 32));
}

void QInt::setbit0(int i)
{
	m_arr[3 - i / 32] = (~(1 << (i % 32))) & m_arr[3 - i / 32];
}

QInt QInt::getTowComplement()
{
	QInt rs;
	for (int i = 0; i < 4; i++) {
		rs.m_arr[i] = ~this->m_arr[i];
	}

	bool carry = false;
	if (rs.getBit(0) == 1) { carry = true; rs.setbit0(0); }
	else {
		rs.setbit1(0);
	}
	int i = 1;
	while (carry) {
		if (i == 128) break;
		if (rs.getBit(i) == 0) {
			rs.setbit1(i);
			break;
		}
		rs.setbit0(i);
		i++;
	}
	return rs;
}

vector<int> QInt::convert2vt_bin(string str, int base)
{
	/*
	Chuyển string base bất kì (2,10,16) về vector<int> base 2
	*/
	if (base == 2) return str_bin2bin(str);
	else if (base == 10) return str_dec2bin(str);
	else if (base == 16) return str_hex2bin(str);
}

vector<int> QInt::str_hex2bin(string str)
{
	/*
	Chuyển string ở base 16 về vector<int> base 2
	bit phải nhất sẽ lưu ở vị trí [0] để tiện các thao tác xử lí sau
	vd: "ab" chuyển bình thường: 1010 1011
			 chuyển qua hàm:     1101 0101
	*/
	vector<int> ans;
	vector<int> vtBin;
	map<char, int> mHex_char = { {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5},{'6', 6}, {'7', 7},
								{'8', 8}, {'9', 9}, {'A', 10}, {'B', 11},{'C', 12}, {'D', 13}, {'E', 14}, {'F', 15} };
	int dec_val = 0;
	for (auto ch : str) {		//mỗi vòng sẽ lấy một kí tự hexa chuyển về nhị phân, lưu vào vtBin
		dec_val = mHex_char[toupper(ch)];
		vtBin.clear();
		while (dec_val > 0) {
			vtBin.push_back(dec_val % 2);
			dec_val /= 2;
		}
		while (vtBin.size() < 4) vtBin.insert(vtBin.end(), 0);
		ans.insert(ans.begin(), vtBin.begin(), vtBin.end());		//insert vtBin theo thứ tự ngược, để cuối cùng bit phải nhất của kết quả sẽ ở ans[0]
	}
	return ans;
}

vector<int> QInt::str_bin2bin(string str)
{
	/*
	Chuyển string base 2 về vector<int> base 2
	bit phải nhất sẽ lưu ở vị trí [0] để tiện các thao tác xử lí sau
	*/
	vector<int> ans;
	for (auto i : str)
		ans.insert(ans.begin(), i - '0');
	return ans;
}

string QInt::devide_strBy2(string str)
{
	/*
	Chia một string base 10 cho 2
	*/
	string ans;
	int quotient = 0;
	int remainder = 0;
	int divident = 0;
	for (int i = 0; i < str.size(); i++) {
		divident = (remainder * 10 + str[i] - '0');
		ans.push_back(divident / 2 + '0');
		remainder = divident % 2;
	}
	ans = ans[0] == '0' ? ans.substr(1) : ans;
	return ans;
}

vector<int> QInt::str_dec2bin(string str)
{
	/*
	Chuyển string base 10 về vector<int> base 2
	bit phải nhất sẽ lưu ở vị trí [0] để tiện các thao tác xử lí sau
	điều kiện input: nếu là số dương thì không có kí tự dấu ở đầu
					 nếu là số âm thì có kí tự '-' ở đầu
	*/
	vector<int> ans;
	int dec_val = 0;
	bool is_nega = false;		//âm, dương

	if (str[0] == '-') {
		is_nega = true;
		str = str.substr(1);				//bỏ qua str[0]
	}

	while (str.size() > 0) {
		ans.push_back((str.back() - '0') % 2);			//Để xử lí những số quá lớn thì dùng chia string
		str = devide_strBy2(str);
	}

	ans.insert(ans.end(), 128 - ans.size(), 0);			//điền toàn bộ bit 0 còn lại 
	if (is_nega) {
		for (int i = 0; i < ans.size(); i++)	//chuyển sang số bù 1
			ans[i] = !ans[i];
		if (ans[0] == 0) ans[0] = 1;			//nếu bit đầu bằng 0 thì cộng 1 và kết thúc
		else {									//bit đầu là 1 và có nhớ
			ans[0] = 0;
			for (int i = 1; i < ans.size(); i++)
				if (ans[i] == 0) {				//nếu bit i là 0 thì nhớ 1 và kết thúc
					ans[i] = 1;
					break;
				}
				else ans[i] = 0;				//bit i là 1 thì nhớ 1 -> 0 và tiếp tục
		}
	}
	return ans;
}
