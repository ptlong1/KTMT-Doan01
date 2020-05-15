#include "QFloat.h"

QFloat& QFloat::operator=(const QFloat& other)
{
	if (this != &other) {
		for (int i = 0; i < 4; i++)
			m_arr[i] = other.m_arr[i];
	}
	return *this;
}

QFloat QFloat::DoiDau()
{
	QFloat ans = *this;
	Daobit(ans.m_arr[0], 31);
	return ans;
}

int QFloat::operator[](int i)
{
	if (i > 127 || i < 0) return 0;
	int vti, vtj;
	vti = i / 32;
	vtj = i % 32;
	return Getbit(this->m_arr[4 - 1 - vti], vtj);
}

QFloat QFloat::operator>>(int x)
{
	QFloat ans;
	ans.GanDauvaMu(*this);
	for (int i = 0; i < 112 - x; ++i)
	{
		int vti, vtj;
		vti = 3 - i / 32;
		vtj = i % 32;
		ans.m_arr[vti] += (*this)[i + x]*(1 << vtj);
	}
	return ans;
}

QFloat QFloat::operator<<(int x)
{
	QFloat ans;
	ans.GanDauvaMu(*this);
	//cout << ans.toDec() << '\n';
	for (int i = x; i < 112; ++i)
	{
		int vti, vtj;
		vti = 3 - i / 32;
		vtj = i % 32;
		ans.m_arr[vti] += (*this)[i - x] * (1 << vtj);
	}
	return ans;
}



bool QFloat::operator>=(QFloat other)
{
	for (int i = 111; i >= 0; --i)
	{
		if ((*this)[i] > other[i]) return true;
		if ((*this)[i] < other[i]) return false;
	}
	return true;
}

QFloat QFloat::CongPhanMu(int x)
{
	int nho = 0;
	QFloat ans = *this;
	for (int i = 112; i < 127; ++i)
	{
		int vti, vtj;
		vti = 3 - i / 32;
		vtj = i % 32;
		int tmp = ans[i] + Getbit(x, i - 112) + nho;
		if (tmp % 2 && ans[i] == 0) Batbit(ans.m_arr[vti], vtj);
		if (!(tmp % 2) && ans[i] == 1) Daobit(ans.m_arr[vti], vtj);
		nho = tmp / 2;
	}
	return ans;
}

void QFloat::GanDauvaMu(QFloat x)
{
	for (int i = 127; i >= 112; --i)
	{
		if (x[i] != (*this)[i]) Daobit(this->m_arr[3 - i / 32], i % 32);
	}
}

QFloat::QFloat()
{
	for (int i = 0; i < 4; ++i) m_arr[i] = 0;
}

QFloat QFloat::operator+(QFloat other)
{
	QFloat ans;
	QFloat a, b;
	a = *this;
	b = other;

	//tinh so x trong 2^x cua tung bien
	int mu_1, mu_2;
	mu_1 = mu_2 = 0;
	for (int i = 126; i >= 112; --i)
	{
		mu_1 = mu_1 * 2 + (*this)[i];
		mu_2 = mu_2 * 2 + other[i];
	}
	//day khong phai la so duoi dang bias, ma la so duoi dang unsigned
	//mu_1 -= (1 << 14) - 1; bias
	//mu_2 -= (1 << 14) - 1; bias

	//so sanh, dua ve dang a > b
	if (mu_2 > mu_1 || (mu_1 == mu_2 && b >= a)) swap(a, b), swap(mu_1, mu_2);
	
	//chenh lech so mu
	int mu_diff = mu_1 - mu_2;

	if (mu_diff > 112) return a;
	//dua b ve dang ko chuan hoa
	b = b >> mu_diff;
	int vt1 = 112 - mu_diff;
	//them bit 1 dung dau
	Batbit(b.m_arr[3 - vt1 / 32], vt1 % 32);

	int nho = 0;
	//cung dau
	if (a[127] == b[127])
	{
		ans.GanDauvaMu(a);
		nho = 0;
		for (int i = 0; i < 112; ++i)
		{
			if ((a[i] + b[i] + nho) % 2)
				Batbit(ans.m_arr[3 - i / 32], i % 32);
			nho = (a[i] + b[i] + nho) / 2;
		}
		//neu so b la dang ko chuan
		if (mu_diff > 0)
		{
			if (nho == 1)
			//nho = 1, cong vao so 1 truoc dau phay
			{
				//1.0 10.
				ans = ans >> 1;
				// day so 0 dung dau phan tri
				//Batbit(ans.m_arr[3 - 111 / 32], 111 % 32);
				//cong 1 vao phan mu
				ans = ans.CongPhanMu(1);
				//them vao phan mu 1 don vi
			}
		}
		else
		{
			//neu nhu b o dang chuan, cong 2 so 1 dung dau
			//2 trung hop
			// 11. 10.
			ans = ans >> 1;
			if (nho == 1) // 11.
			{
				Batbit(ans.m_arr[3 - 111 / 32], 111 % 32); // bat bit 1
			}
			ans = ans.CongPhanMu(1);
			//tang phan mu len 1
		}
	}
	else
	{
		//khac dau, a- b
		//key qua cuoi cung lay theo dau a
		ans.GanDauvaMu(a);
		nho = 0;
		for (int i = 0; i < 112; ++i)
		{
			if ((a[i] - b[i] + nho + 2) % 2 == 1)
				Batbit(ans.m_arr[3 - i / 32], i % 32);
			if (a[i] - b[i] + nho < 0) nho = -1;
			else nho = 0;
		}
		if (mu_diff > 0)
		{
			//cout << "***";
			// 1.xxx - 0.xxx
			if (nho == -1)
			//TH tieu bien so 1 truoc dau phay
			// 0.xxxx
			{
				//cout << "****";
				int vt = -1;
				//tim vi tri so 1 gan nhat
				for (int i = 111; i >= 0; --i)
					if (ans[i] == 1)
					{
						vt = i;
						break;
					}
				cout << vt << '\n';
				//dua no ve dang chuan
				ans = ans << (112 - vt);
				//tru di 1 luong tieu ton o phan mu
				ans = ans.CongPhanMu(-(112 - vt));
			}
		}
		else
		{
			//neu ca 2 o dang chuan
			if (nho == 0) // 1.xxx - 1.xxx = 0.xxxx
				// khong co th nho == 1 vi a >= b
			{
				int vt = -1;
				for (int i = 111; i >= 0; --i)
					if (ans[i] == 1)
					{
						vt = i;
						break;
					}
				if (vt == -1) // neu tim khong ra aka 2 so bang nhau
				{
					/*
					for (int i = 126; i > 111; --i)
						if (ans[i] == 1)
							Daobit(ans.m_arr[3 - i / 32], i % 32);
					*/
					QFloat zero;
					return zero;
					//tra ve zero
			}
				else
				{
					ans = ans << (112 - vt);
					ans = ans.CongPhanMu(-(112 - vt));
				}
			}
		}
	}
	return ans;
}

QFloat QFloat::operator-( QFloat other)
{
	return *this + other.DoiDau();
}

QFloat::QFloat(vector<int> vtBin) {
	for (int i = 0; i < vtBin.size(); i++)
		if (vtBin[i] == 1) setbit1(SIZE-1 - i);
		else setbit0(SIZE-1- i);
}

QFloat::QFloat(string str, int base) {
	vector<int> vtBin = convert2vt_bin(str, base);
	for (int i = 0; i < vtBin.size(); i++)
		if (vtBin[i] == 1) setbit1(SIZE - 1 - i);
		else setbit0(SIZE - 1 - i);
}

void QFloat::ScanQFloat(istream& f, int base)
{
	string str;
	getline(f, str);
	QFloat ans(str, base);
	*this = ans;
}

vector<int> QFloat::convert2vt_bin(string str, int base)
{
	/*
	Chuyển string base bất kì (2,10,16) về vector<int> base 2
	*/
	if (base == 2) return str_bin2bin(str);
	else if (base == 10) return str_dec2bin(str);
	else if (base == 16) return str_hex2bin(str);
}

vector<int> QFloat::str_bin2bin(string str)
{
	/*
	Chuyển string base 2 về vector<int> base 2
	bit phải nhất sẽ lưu ở vị trí [0] để tiện các thao tác xử lí sau
	*/
	vector<int> ans;
	for (auto i : str)
		ans.push_back(i - '0');
	return ans;
}

vector<int> QFloat::str_hex2bin(string str)
{
	/*
	Chuyển string ở base 16 về vector<int> base 2
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
			vtBin.insert(vtBin.begin(), dec_val % 2);
			dec_val /= 2;
		}
		while (vtBin.size() < 4) vtBin.insert(vtBin.end(), 0);
		ans.insert(ans.end(), vtBin.begin(), vtBin.end());
	}


	return ans;
}

void QFloat::str_devide2(string& str) {
	/*
	Chia một string base 10 cho 2
	*/
	string ans;
	int remainder = 0;
	int divident = 0;
	for (int i = 0; i < str.size(); i++) {
		divident = (remainder * 10 + str[i] - '0');
		ans.push_back(divident / 2 + '0');
		remainder = divident % 2;
	}
	ans = ans[0] == '0' ? ans.substr(1) : ans;
	str = ans;
}

void QFloat::str_multi2(string& str) {
	/*
	Nhân một string dạng "03456" tức là 0.3456
	*/
	int tmp = 0;
	int carry = 0;
	for (int i = str.size() - 1; i >= 0; i--) {
		tmp = (str[i] - '0') * 2 + carry;
		carry = tmp >= 10 ? 1 : 0;
		str[i] = tmp % 10 + '0';
	}
	auto a = str.find_last_of('0');

	while (str.back() == '0') str.pop_back();    //xÓa các kí tự '0' ở cuối
}

vector<int> QFloat::str_dec2bin(string str) {
	/*
	Chuyển string base 10 về vector<int> base 2
	điều kiện input: nếu là số dương thì không có kí tự dấu ở đầu
					 nếu là số âm thì có kí tự '-' ở đầu
	output: <1 bit dấu> <15 bit mũ> <112 bit trị>
	*/

	vector<int> ans;
	vector<int> exponent;
	vector<int> snfcant;

	int sign_bit = 0;		//bit dấu
	//xử lí phần dấu
	if (str[0] == '-') {
		sign_bit = 1;
		str = str.substr(1);				//bỏ qua str[0]
	}

	//xử lí phần significant
	long pos = str.find('.');					//pos = -1 tức là k có dấu chấm
	string int_part = str.substr(0, pos);
	string frac_part = pos == -1 ? "" : str.substr(pos + 1, string::npos);
	while (frac_part.size() > 0 && frac_part.back() == '0') frac_part.pop_back();//bỏ những số 0 thừa phía sau phần thập phân
	while (int_part.size() > 0 && int_part.front() == '0') int_part.erase(int_part.begin()); //xóa bit 0 thừa ở đằng trước phần nguyên

	while (int_part.size() > 0) {
		snfcant.insert(snfcant.begin(), (int_part.back() - '0') % 2);
		str_devide2(int_part);						//Để xử lí những số quá lớn thì dùng chia string
	}

	long point_pos = snfcant.size();		//vị trí phân tách phần nguyên và thập phân
	frac_part.insert(0, "0");		//thêm 0 vào đầu string phần thập phân

	while (frac_part.size() > 1 && snfcant.size() < pow(2, EXPONENT_SIZE - 1) + 1) {//số bit tối đa lấy được chính là số mũ tối đa, nhìn xuống phần tính mũ, dịch qua một bit là tăng lên 1 mũ
		str_multi2(frac_part);
		snfcant.push_back(frac_part[0] - '0');
		frac_part[0] = '0';
	}//phần trị bây giờ có số bit khá nhiều, chưa giới hạn

	//xử lí exponent
	long exponent_val = 0;
	if (snfcant.size() == 0) exponent_val = -pow(2, EXPONENT_SIZE - 1) + 1;
	else {
		if (point_pos == 0) {		//không có phần nguyên thì cần dịch dấu chấm phần trị qua phải đến bit 1 gần nhất
			while (snfcant.front() == 0 && exponent_val > (-pow(2, EXPONENT_SIZE - 1) + 2)) {//số dạng không chuẩn thì ở phần mũ biểu diễn là 000...0 tức là -2^(n-1)+1, nhưng khi tính ra giá trị thì lấy -2^(n-1)+2 nên phải xử lí một vài chỗ để thỏa mãn
				snfcant.erase(snfcant.begin());				//mỗi lần dịch thì xóa bit 0 không cần thiết 
				exponent_val--;
			}
			exponent_val--;
		}
		else exponent_val = point_pos - 1;	//có phần nguyên thì chuyển dấu chấm đến sau bit đầu tiên phần trị(bit đầu tiên luôn là 1)
		if (exponent_val != -pow(2, EXPONENT_SIZE - 1) + 1) snfcant.erase(snfcant.begin());		//xóa bit 1 mặc định ở phần trị, nếu là số không chuẩn thì k xóa
	}
	if (exponent_val >= pow(2, EXPONENT_SIZE - 1)) {			//nếu số mũ >= 2^(n-1) thì tức là inf=>xóa phần trị (sau đó sẽ đc fill toàn 0)
		exponent_val = pow(2, EXPONENT_SIZE - 1);
		snfcant.clear();
	}

	//chuyển mũ về số bias
	exponent_val += pow(2, EXPONENT_SIZE - 1) - 1;
	while (exponent_val > 0) {
		exponent.insert(exponent.begin(), exponent_val % 2);
		exponent_val /= 2;
	}

	while (exponent.size() < EXPONENT_SIZE) exponent.insert(exponent.begin(), 0);		//điền các bit 0 vào đầu để đủ số bit phần mũ
	while (snfcant.size() < SIGNIFICANT_SIZE) snfcant.push_back(0);		//điền các bit 0 vào đầu để đủ số bit phần trị
	ans.insert(ans.begin(), sign_bit);
	ans.insert(ans.end(), exponent.begin(), exponent.end());
	ans.insert(ans.end(), snfcant.begin(), snfcant.end());

	if (ans.size() < SIZE) ans.insert(ans.end(), SIZE - ans.size(), 0);			//điền toàn bộ  bit còn trống phía sau là 0 
	ans.resize(SIZE);															//giỚi hạn lại số bit
	return ans;
}




QFloat QFloat::getZero()
{
	return QFloat();
}

QFloat QFloat::getInf()
{
	QFloat x;
	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		x.setbit1(i);
	}
	return x;
}

QFloat QFloat::getNaN()
{
	QFloat x;
	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		x.setbit1(i);
	}
	x.setbit1(111);
	return x;
}


QFloat::~QFloat()
{
}

int QFloat::getSign() const
{
	return this->getBit(127);
}

int QFloat::getExp()
{
	string exp(15, '0');
	for (int i = 112; i < 127; i++)
	{
		exp[126 - i] = this->getBit(i) + '0';
	}
	int x = 0;
	for (int i = 0; i < 15; i++) {
		if (exp[i] == '1')
			x = x | (1 << (14 - i));
	}
	return x - 16383;//2^14 - 1 = 16383
}

string QFloat::getSignificand()
{

	int i = 0;
	while (i < 112) {
		if (this->getBit(i) == 1)
			break;
		i++;
	}
	if (i == 112)
		return "0";

	string significand(112 - i, '0');
	for (; i < 112; i++)
	{
		significand[111 - i] = this->getBit(i) + '0';
	}

	return significand;
}

int QFloat::getBit(int i) const
{
	i = i % 128;
	return ((m_arr[3 - i / 32] >> (i % 32)) & 1);
}

void QFloat::setbit1(int i)
{
	m_arr[3 - i / 32] = m_arr[3 - i / 32] | (1 << (i % 32));
}

void QFloat::setbit0(int i)
{
	m_arr[3 - i / 32] = (~(1 << (i % 32))) & m_arr[3 - i / 32];
}

int QFloat::checkExponent()
{
	int exp = this->getExp();
	if (exp == -16383) return 0;
	if (exp == 16383) return 1;
	return -1;
}



string QFloat::toBin()
{
	if (this->isNaN())
		return "NaN";

	if (this->isInf())
		return "Inf";

	if (this->isZero())
		return "0";

	char sign = this->getSign(); //phan dau
	string significand = this->getSignificand(); //phan tri
	int exp = this->getExp(); //phan mu
	string rs = "1";

	if (checkExponent() == 0)
	{
		/*------------bieu dien so khog chuan----------*/
		rs = string(-exp + 1, '0') + significand;
		rs[1] = '.';
		if (sign == 1) rs = '-' + rs;
		return rs;
	}

	if (significand.size() != SIGNIFICANT_SIZE) {
		significand += string(SIGNIFICANT_SIZE - significand.size(), '0');
	}
	rs += significand;

	//ko phai 2 truong hop tren
	if (exp >= 0)
	{

		if (exp < 112) {
			if (exp + 1 != rs.size())
				rs.insert(exp + 1, ".");
		}
		else if (exp > 112)
		{
			string num0(exp - 112, '0');
			rs += num0;
		}
	}
	else
	{
		string num0(-exp + 1, '0');
		num0[1] = '.';
		rs = num0 + rs;
	}
	if (sign == 1)
		rs.insert(0, "-");
	return rs;
}

string QFloat::toBin_t() {
	string ans;
	for (int i = 0; i < SIZE; i++)
		ans.insert(0, 1, getBit(i) + '0');
	return ans;
}

string QFloat::toDec()
{
	if (this->isNaN())
		return "NaN";

	if (this->isInf())
		return "Inf";

	if (isZero())
		return "0";

	char sign = this->getSign();
	string significand = this->getSignificand();
	int exp = this->getExp();
	string rs = "1";

	if (checkExponent() == 0)
	{
		/*------------bieu dien so khog chuan----------*/
		exp += 1; // exp = -2^14 + 2
		rs[0] = '0';
	}

	int x = 0;
	string temp = rs + significand;
	while (true) {
		if (temp[x] == '1')
			break;
		x++;
	}

	temp.erase(0, x); //loai bo cac so 0 dung dau vo nghia

	//ko phai 2 truong hop tren
	int expNew = exp - significand.size();//vi dich chuyen dau phay qua tan cung ben phai trong phan tri
	QInt numInt;
	//cout << significand << '\n';
	numInt = temp;
	//cout << numInt.Dec() << '\n';
	BigNumber numDec(numInt.toDec());
	numDec.multiPow2(expNew);
	//cout << numDec.getNum() << '\n';
	rs.clear();
	if (sign == 1) {
		rs += '-';
	}

	rs += numDec.getNum();

	return rs;
}

void QFloat::PrintQFloat(ostream& f, int base)
{
	string ans;
	if (base == 2) f << this->toBin()<<endl;
	else if (base == 10) f << this->toDec()<<endl;
}

string QFloat::toString(int base) {
	if (base == 2) return this->toBin_t();
	else return this->toDec();
}
QFloat QFloat::operator/(const QFloat& other)
{
	QFloat temp = other;
	
	if (temp.isNaN() || this->isNaN())
		return getNaN();

 	if (this->isInf()) {
		if (temp.isInf()){
			return getNaN();
		}
		return getInf();
	}
	
	//Kiem tra chia 0
	if (temp.isZero()) {
		return getNaN();
	}

	//co la vo cung hay k

	if (this->isZero()) {
		return getZero();
	}

	/*-----------------------------------------------------*/
	int exp1 = this->getExp();
	int exp2 = temp.getExp();

	int newExp = exp1 - exp2;
	
	if (newExp >=EXPONENT_BIAS_VALUE + 1) {
		return getInf();
	} 

	if (newExp < -EXPONENT_BIAS_VALUE + 2 - SIGNIFICANT_SIZE) {
		return getZero();
	}

	/*--------------------------------------------------------*/

 	string significand1 = this->getSignificand();
	string significand2 = temp.getSignificand();
	/*if (significand1.size() < exp1)
		significand1 += string(exp1 - significand1.size(), '0');
	if (significand2.size() < exp2)
		significand2 += string(exp2 - significand2.size(), '0');*/

	string value1 = "0", value2 = "0";

	if (exp1 != -EXPONENT_BIAS_VALUE)
		value1 = "1";
	if (exp2 != -EXPONENT_BIAS_VALUE) 
		value2 = "1";
	

	int e_snf12 = significand1.size() - significand2.size();
	/*if (significand1 == "0") 
		e_snf12 += 1;
	else*/
		value1 += significand1; 

	/*if (significand2 == "0")
		e_snf12 -= 1;
	else*/
		value2 += significand2;

	//Chia phan tri
	QFloat div_snf;
	div_snf = divideSignificand(value1, value2);
	
	//Xu ly dau phay phan tri
	int exp_snf = div_snf.getExp();

	//Xu ly phan mu (exp va phan mu trong phan tri)
	newExp += (exp_snf) - e_snf12 + EXPONENT_BIAS_VALUE;
	
	/*-----------------------------------------------------------------------------*/
	QFloat rs;
	rs = div_snf;
	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		if ((newExp >> (i - 112)) & 1 == 1)
			rs.setbit1(i);
		else
			rs.setbit0(i);
	}

	if (this->getSign() != temp.getSign())
		rs.setbit1(127);

	
	return rs;
}

QFloat QFloat::divideSignificand(string snf1, string snf2)
{
	QInt numInt1(snf1, 2), numInt2(snf2, 2);
	pair<QInt, QInt> q_r[20];
	q_r[0] = numInt1 / numInt2;
	
	string value = q_r[0].first.toBin();
	int exp_snf = value.size()-1;

	/*Chia lay thuong co nhieu hon 112 so co nghia*/
	for (int i = 1; i < 20; i++) {
		string  tmp;
		q_r[i] = (q_r[i - 1].second << 15) / numInt2; //Dich phai 15 bit (*2^15)
		tmp = q_r[i].first.toBin();
		if (tmp.size() < 15) 
			tmp = string(15 - tmp.size(), '0') + tmp;
		value += tmp;
	}

	//Xử lý phần mũ dư ra
	exp_snf += /*-exp_proccess(snf1) + exp_proccess(snf2)*/ - exp_proccess(value);//2^exp_snf
	/*if (snf2 == "1") {
		exp_snf += snf1.size() - 1;
	}*/
	exp_snf = exp_snf + EXPONENT_BIAS_VALUE;
	
	//Xử lý đưa về dạng formalize: xxxxxx
	int pos1_value = value.find('1', 0);
	if (pos1_value != -1)
		value.erase(0, pos1_value + 1);
	
	if (value.size() < SIGNIFICANT_SIZE)
		value += string(SIGNIFICANT_SIZE - value.size(), '0');
	else if (value.size() > SIGNIFICANT_SIZE)
		value.resize(SIGNIFICANT_SIZE);

	//Khoi tao ket qua
	QFloat rs;
	
	for (int i = 0; i < SIGNIFICANT_SIZE; i++) {
		if (value[SIGNIFICANT_SIZE -1- i] == '1')
			rs.setbit1(i);
	}

	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		if ((exp_snf >> (i - SIGNIFICANT_SIZE)) & 1 == 1)
			rs.setbit1(i);
	}

	/*QFloat x = rs * QFloat("0.45703125", 10);
	cout << x.toDec() << endl;*/

	return rs;

}

int QFloat::exp_proccess(string snf)
{
	if (snf[0] == '0') 
		return snf.find('1', 0);

	return 0;
}

void QFloat::setNaN() {
	for (int i = 0; i < EXPONENT_SIZE; i++)		//phần mũ toàn 1
		setbit1(SIZE - i - 2);
	setbit1(0);								//phần trị khác 0
}
void QFloat::setZero() {
	for (int i = 0; i < SIZE; i++)					//set tất cả các bit là 0
		setbit0(i);
}
void QFloat::setInf(int sign) {
	if (sign == 1) setbit1(SIZE - 1);
	else setbit0(SIZE - 1);
	for(int i=0;i<EXPONENT_SIZE; i++)
		setbit1(SIZE - 2 - i);
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		setbit0(SIZE - 2 - EXPONENT_SIZE - i);
}
bool QFloat::isZero() const {
	for (int i = 0; i < SIZE-1; i++)
		if (getBit(i) != 0) return false;
	return true;
}
bool QFloat::isDenorm() const {
	for (int i = 0; i < EXPONENT_SIZE; i++)
		if (getBit(SIZE - 2 - i) != 0) return false;
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		if (getBit(SIZE - 2 - EXPONENT_SIZE - i) != 0) return true;
	return false;
}
bool QFloat::isInf() const {
	for (int i = 0; i < EXPONENT_SIZE; i++)
		if(getBit(SIZE - 2 - i)!=1) return false;
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		if(getBit(SIZE - 2 - EXPONENT_SIZE - i)!=0) return false;
	return true;
}
bool QFloat::isNaN() const {
	for (int i = 0; i < EXPONENT_SIZE; i++)		
		if(getBit(SIZE - i - 2)!=1) return false;
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		if (getBit(SIZE - 2 - EXPONENT_SIZE - i) != 0) return true;
	return false;
}


bool QFloat::check_all_bit0(vector<int> vt) const {
	for (int i = 0; i < vt.size(); i++)
		if (vt[i] != 0) return false;
	return true;
}
bool QFloat::check_all_bit1(vector<int> vt) const {
	for (int i = 0; i < vt.size(); i++)
		if (vt[i] != 1) return false;
	return true;
}
void QFloat::set_all_bit0(vector<int>& vt) {
	for (int i = 0; i < vt.size(); i++)
		vt[i] = 0;
}
void QFloat::set_all_bit1(vector<int>& vt) {
	for (int i = 0; i < vt.size(); i++)
		vt[i] = 1;
}
vector<int> QFloat::get_exponent() const {
	/*
	Lấy các bit phần mũ
	*/
	vector<int> ans;
	for (int i = 0; i < EXPONENT_SIZE; i++)
		ans.push_back(getBit(SIZE - 2 - i));		
	return ans;
}
vector<int> QFloat::get_snfcant() const {
	/*
	Lấy các bit phần trị
	*/
	vector<int> ans;
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		ans.push_back(getBit(SIZE - 2 - EXPONENT_SIZE - i));
	return ans;
}
vector<int> QFloat::get_full_snfcant() const {
	/*
	Lấy các bit phần trị và thêm bit mặc định (0 cho dạng không chuẩn, và 1 cho dạng chuẩn)
	*/
	vector<int> ans = get_snfcant();
	ans.insert(ans.begin(), isDenorm() ? 0 : 1);
	return ans;
}
vector<int> QFloat::multi_snfcant(vector<int> s1, vector<int> s2, int& carry0) {
	/*
	Nhân hai vector phần snfc theo thuật toán booth
	Input: phần snfc đầy đủ (đã thêm 1 hoặc 0 tùy vào normal hay denormal)
	Output: vector<int>. Giả sử kết quả của phép nhân là ABCDE thì hiểu phần trị có dạng A.BCDE
	*/
	vector<int> ans;
	int carry = 0;
	int sum = 0;
	ans.assign(1 + SIGNIFICANT_SIZE + 1, 0);		//ans[0]=0 là  bit nhớ phụ
	ans.insert(ans.end(), s2.begin(), s2.end());
	for (int i = 0; i < SIGNIFICANT_SIZE + 1; i++) {
		if (ans.back() == 1) {
			//Cộng s1
			for (int j = SIGNIFICANT_SIZE + 1; j >= 1; j--) {
				sum = ans[j] + s1[j - 1] + carry;
				carry = sum > 1 ? 1 : 0;
				ans[j] = sum % 2;
			}
			ans[0] += carry;
		}
		ans.pop_back();
		ans.insert(ans.begin(), 0);
	}		//kết quả có dạng 0abcde với độ dài là (SIGNIFICANT_SIZE+1)*2, đc hiểu là 0ab.cde
	ans.erase(ans.begin()); //xóa bit nhớ phụ, còn lại ab.cde
	if (ans[0] == 1) carry0 = 1;		//Nếu a = 1 tức là phải normalize thành a.bcde và tăng số mũ lên 1(carry)
	else ans.erase(ans.begin());	//Còn không thì xóa bit a và kết quả là b.cde
	return ans;
}
vector<int> QFloat::add_exponent(vector<int> exp1, vector<int> exp2, int carry0) {
	/*
	Cộng hai vector phần mũ dạng bias
	Output: hai vector chứa các bit phần mũ hai số
	Output: vector kết quả trong đó vị trí [0] chứa flag
			-3: phần mũ = -2^(n-1)+1 =>underflow
			-2: phần mũ <-2^(n-1)+1  =>underflow
			-1: phần mũ không bị tràn
			 2: phần mũ >2^(n-1)-1	 =>overflow
	*/
	vector<int> ans;
	vector<int> bias;
	bias.assign(EXPONENT_SIZE, 1);
	bias[0] = 0;		//số bias
	int flag = -1; //cờ mặc định là không thuộc trường hợp đặc biệt
	int sum = 0;
	int carry = carry0;
	for (int i = EXPONENT_SIZE - 1; i >= 0; i--) {			//cộng hai phần dấu
		sum = exp1[i] + exp2[i] - bias[i] + carry;
		if (sum > 1) carry = 1;
		else if (sum < 0) carry = -1;
		else carry = 0;
		ans.insert(ans.begin(), (sum + 2) % 2);
	}

	if (exp1[0] == exp2[0] && exp1[0] != ans[0]) {				//nếu cùng dấu mà cộng ra số trái dấu =>tràn số
		if (exp1[0] == 0 && ans[0] == 1)						//âm+âm ra dương => underflow	
			flag = -2;
		else if (exp1[0] == 1 && ans[0] == 0)					//dương+dương ra âm=>overflow
			flag = 2;
	}
	else if (check_all_bit0(ans))								//mũ bằng 000..0 = -2^(n-1)+1 => underflow 
		flag = -3;
	else if (check_all_bit1(ans))								//mũ bằng 111..1 = 2^(n-1) => overflow
		flag = 2;

	ans.insert(ans.begin(), flag);
	return ans;
}
void QFloat::handle_multi_special_case(int flag, vector<int>& exp, vector<int>& snf)
{
	/*
	Xử lí các trường hợp đặc biệt như underflow, overflow
	Input:  flag: cờ, -3=>  underflow, số mũ = -2^(n-1)+1
					  -2=>	underflow, số mũ < -2^(n-1)+1
					  -1=>	bình thường
					   2=>	overflow	
		    exp: vector<int> các bit phần mũ
		    snf: vector<int> các bit phần trị
	*/
	if (flag == -2 || flag == -3) {	//underflow
		//tính giá trị mũ
		long long exp_val = 0;
		for (int i = 0; i < EXPONENT_SIZE; i++)
			exp_val += exp[i] * pow(2, EXPONENT_SIZE - i - 1);
		exp_val -= pow(2, EXPONENT_SIZE - 1) - 1;

		int pass_over = flag == -2 ? pow(2, EXPONENT_SIZE - 1) - exp_val + 2 : 1;	//khi vượt ngoài phạm vi biểu diễn(tràn số) thì giá trị sẽ quay vòng lại, nên có công thức như vậy để tính số giá trị đã quay vòng; riêng TH -2^(n-1)+1 không quay vòng nhưng vẫn tính tràn 
		set_all_bit0(exp);						//đưa phần dấu về toàn bit 0, thể hiện số denormal
		snf.insert(snf.begin(), pass_over, 0);		//dịch dấu chấm qua trái 	
	}
	else if (flag == 2) {		//overflow
		set_all_bit1(exp);
		set_all_bit0(snf);
	}
}
QFloat QFloat::operator*(const QFloat& other)
{
	/*
	Phép nhân
	*/
	QFloat ans;
	int flag1 = this->isZero() ? 1 : this->isInf() ? 3 : this->isNaN() ? -1 : 5; 		//NaN là -1, zero là 1, inf là 3, còn lại là 5
	int flag2 = other.isZero() ? 1 : other.isInf() ? 3 : other.isNaN() ? -1 : 5;

	if (flag1*flag2 < 0) ans.setNaN();		//một trong hai số là NaN
	else if (flag1*flag2 == 5) ans.setZero();	//zero*binhthuong = zero
	else if (flag1*flag2 == 3) ans.setNaN();   //zero*inf=>NaN
	else if (flag1*flag2 == 15) {			//inf*binhthuong = (dau bth)inf
		if (this->isInf()) ans.setInf(this->getSign());
		else ans.setInf(other.getSign());
	}
	else {								//cả hai số đều bình thường
		int carry = 0;			//carry nếu phần trị nhân vượt quá 2 và phải normalize
		vector<int> snfcant_ans = multi_snfcant(this->get_full_snfcant(), other.get_full_snfcant(), carry);		//nhân phần trị
		vector<int> exponent_ans = add_exponent(this->get_exponent(), other.get_exponent(), carry);	//cộng phần dấu


		//xử lí các trường hợp tràn số lớn, tràn số nhỏ
		int flag = exponent_ans[0];
		exponent_ans.erase(exponent_ans.begin());		//xóa flag
		handle_multi_special_case(flag, exponent_ans, snfcant_ans);

		vector<int> vt_ans;
		vt_ans.assign(1, getSign() ^ other.getSign());			//insert phần dấu; cùng dấu thì ra dương, khác dấu ra âm
		vt_ans.insert(vt_ans.end(), exponent_ans.begin(), exponent_ans.end());	//insert phần mũ
		vt_ans.insert(vt_ans.end(), snfcant_ans.begin() + 1, snfcant_ans.begin() + SIGNIFICANT_SIZE + 1);		//insert phần trị, lấy số bit trong phạm vi ở phần trị
		QFloat tmp(vt_ans);
		ans = tmp;
	}
	return ans;
}


