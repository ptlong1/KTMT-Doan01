#include "QFloat.h"




QFloat::QFloat(vector<int> vtBin) {
	for (int i = 0; i < vtBin.size(); i++)
		if (vtBin[i] == 1) setbit1(127 - i);
		else setbit0(127 -i);
}

QFloat::QFloat(string str, int base) {
	vector<int> vtBin = convert2vt_bin(str, base);
	for (int i = 0; i < vtBin.size(); i++)
		if (vtBin[i] == 1) setbit1(127 - i);
		else setbit0(127 - i);
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
		while (vtBin.size() < 4) vtBin.insert(vtBin.begin(), 0);
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

	while (frac_part.size() > 1 && snfcant.size() < SIGNIFICANT_SIZE * 2) {//lấy thập phân nhiều cho chắc, giới hạn lại sau
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

vector<int> QFloat::get_exponent() const {
	vector<int> ans;
	for (int i = 126; i >= 112; i--)
		ans.push_back(getBit(i));		//vì bit mũ bắt đầu từ bit số 1
	return ans;
}
vector<int> QFloat::get_snfcant() const {
	vector<int> ans;
	for (int i = SIGNIFICANT_SIZE - 1; i >= 0; i--)
		ans.push_back(getBit(i));
	return ans;
}
vector<int> QFloat::multi_snfcant(vector<int> s1, vector<int> s2) {
	vector<int> ans;
	s1.insert(s1.begin(), 1);		//trả lại bit 1 mặc định
	s2.insert(s2.begin(), 1);		//trả lại bit 1 mặc định
	int carry = 0;
	int sum = 0;
	ans.assign(1 + SIGNIFICANT_SIZE + 1, 0);		//ans[0] là  bit nhớ phụ
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
	}
	while (ans.size() > 0 && ans.front() == 0) ans.erase(ans.begin());
	if (ans[0] == 1) ans.erase(ans.begin());
	if (ans.size() < SIGNIFICANT_SIZE) ans.insert(ans.end(), SIGNIFICANT_SIZE - ans.size(), 0);
	ans.resize(SIGNIFICANT_SIZE);
	return ans;
}

bool QFloat::check_all_bit0(vector<int> vt) {
	for (int i = 1; i < vt.size(); i++)
		if (vt[i] != 0) return false;
	return true;
}

void QFloat::set_all_bit0(vector<int>& vt) {
	for (int i = 0; i < vt.size(); i++)
		vt[i] = 0;
}
vector<int> QFloat::add_exponent(vector<int> exp1, vector<int> exp2) {
	vector<int> ans;
	vector<int> bias;
	bias.assign(EXPONENT_SIZE, 1);
	bias[0] = 0;

	int carry = 0;
	int sum = 0;
	for (int i = EXPONENT_SIZE - 1; i >= 0; i--) {			//cộng hai phần dấu
		sum = exp1[i] + exp2[i] - bias[i] + carry;
		if (sum > 1) carry = 1;
		else if (sum < 0) carry = -1;
		else carry = 0;
		ans.insert(ans.begin(), (sum + 2) % 2);
	}

	if (exp1[0] == exp2[0] && exp1[0] != ans[0]) {				//nếu cùng dấu mà cộng ra số trái dấu
		if (exp1[0] == 0 && ans[0] == 1)			//âm+âm ra dương => underflow			
			ans.insert(ans.begin(), -2);
		else if (exp1[0] == 1 && ans[0] == 0) ans.insert(ans.begin(), 2);	//dương+dương ra âm=>overflow
	}
	else if (check_all_bit0(ans))//cộng ra -2^(n-1)+1 vẫn là underflow 
		ans.insert(ans.begin(), -3);
	else ans.insert(ans.begin(), -1);				//không thuộc trường hợp đặc biệt
	return ans;
}

void QFloat::handle_multi_special_case(int flag, vector<int>& exp, vector<int>& snf)
{
	int exp_val = 0;
	for (int i = 0; i < EXPONENT_SIZE; i++)
		exp_val += pow(2, EXPONENT_SIZE - i - 1);
	exp_val -= pow(2, EXPONENT_SIZE - 1) - 1;

	if (flag == -2 || flag == -3)	//underflow
	{
		int pass_over = flag == -2 ? pow(2, EXPONENT_SIZE - 1) - exp_val + 1 : 1;//nếu cộng ra -2(n-1)+1 thì vẫn bị underflow và phải bắn thêm 1 mũ qua phần trị
		set_all_bit0(exp);
		snf.insert(snf.begin(), 1);		//trả lại bit 1 mặc định
		snf.insert(snf.begin(), pass_over, 0);		//dịch dấu chấm qua trái 
		snf.resize(SIGNIFICANT_SIZE);			//lấy các bit trong phạm vi
	}
}
QFloat QFloat::operator *(const QFloat& other)
{
	if (this->isZero() || other.isZero()) {
		QFloat ans("0", 10);
		return ans;
	}

	vector<int> exponent_ans = add_exponent(this->get_exponent(), other.get_exponent());	//cộng phần dấu
	vector<int> snfcant_ans = multi_snfcant(this->get_snfcant(), other.get_snfcant());		//nhân phần trị

	//xử lí các trường hợp đặc biệt: tràn số lớn, tràn số nhỏ, 0 nhân inf
	int flag = exponent_ans[0];
	exponent_ans.erase(exponent_ans.begin());
	handle_multi_special_case(flag, exponent_ans, snfcant_ans);

	vector<int> vt_ans;
	vt_ans.assign(1, this->getBit(127) ^ other.getBit(127));			//cùng dấu thì ra dương, khác dấu ra âm
	vt_ans.insert(vt_ans.end(), exponent_ans.begin(), exponent_ans.end());
	vt_ans.insert(vt_ans.end(), snfcant_ans.begin(), snfcant_ans.end());
	QFloat ans(vt_ans);
	return ans;
}


/*---------------------------------------------------------*/
/*---------------------------------------------------------*/
QFloat QFloat::getZero()
{
	return QFloat();
}

QFloat QFloat::getInf()
{
	QFloat x;
	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		setbit1(i);
	}
	return x;
}

QFloat QFloat::getNaN()
{
	QFloat x;
	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		setbit1(i);
	}
	setbit1(111);
	return QFloat();
}

QFloat::QFloat()
{
	for (int i = 0; i < 4; i++)
		m_arr[i] = 0;
}

QFloat::~QFloat()
{
}

int QFloat::getSign()
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

bool QFloat::isZero() const
{
	for (int i = 1; i < 4; i++)
		if (m_arr[i] != 0) return false;
	if (m_arr[0] != 0 && m_arr[0] != pow(2, 31)) return false; //bit dấu có thể là 0 hoặc 1
	return true;
}

bool QFloat::isInf() const
{
	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		if (getBit(i) != 1) return false;
	}
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		if (getBit(i) == 1) return false;

	return true;;
}

bool QFloat::isNaN() const
{
	for (int i = SIGNIFICANT_SIZE; i < SIGNIFICANT_SIZE + EXPONENT_SIZE; i++) {
		if (getBit(i) != 1) return false;
	}
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		if (getBit(i) == 1) return true;
	return false;
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
	
	temp.erase(0,x); //loai bo cac so 0 dung dau vo nghia

	//ko phai 2 truong hop tren
	int expNew = exp - significand.size();//vi dich chuyen dau phay qua tan cung ben phai trong phan tri
	QInt numInt;
	numInt = temp;
	BigNumber numDec(numInt.toDec());
	numDec.multiPow2(expNew);
	
	rs.clear();
	if (sign == 1) {
		rs += '-';
	}

	rs += numDec.getNum();

	return rs;
}

void QFloat::PrintQFloat(ostream & f, int base)
{
	string ans;
	if (base == 2) f << this->toBin();
	else if (base == 10) f << this->toDec();
}

QFloat QFloat::operator/(const QFloat & other)
{
	QFloat temp = other;
	
	if (temp.isNaN() || this->isNaN())
		return getNaN();

	//Kiem tra chia 0
	if (temp.isZero()) {
		return getNaN();
	}

	//co la vo cung hay k
	if (this->isInf()) {
		if (temp.isInf()){
			return getNaN();
		}
		return getInf();
	}

	if (this->isZero()) {
		return getZero();
	}

	/*-----------------------------------------------------*/
	int exp1 = this->getExp();
	int exp2 = temp.getExp();

	int newExp = exp1 - exp2;
	
	if (newExp > 16384 - 112) {
		//thong bao overflow
	} 

	if (newExp < -16382 - 112) {
		//thong bao underflow
	}
	/*--------------------------------------------------------*/

 	string significand1 = this->getSignificand();
	string significand2 = temp.getSignificand();
	string value1 = significand1, value2 = significand2;

	if (exp1 != -16383) {
		value1 = "1" + value1;
	}
	if (exp2 != -16383) {
		value2 = "1" + value2;
	}

	QInt numInt1(value1, 2), numInt2(value2, 2);
	pair<QInt, QInt> q_r[8];
	q_r[0] = numInt1 / numInt2;
	
	string value = q_r[0].first.toBin();

	for (int i = 1; i < 8; i++) {
		q_r[i] = q_r[i - 1].second / numInt2;
		value += q_r[i].first.toBin();
 	}

	int e1 = significand1.size() - significand2.size();
	int e2 = q_r[0].first.toBin().size();
	int e3;

	int pos1 = value.find('1', 0);
	if (pos1 != 0) {
		e3 = -pos1 + e1;
	}
	else {
		e3 = -pos1 + e1 - (e2 - 1);
	}
	
	newExp += e3;

	if (newExp < -16382) {
		e3 = -16382 - newExp;
		newExp = -16383; //-> 000...00
		value = string(e3, '0');
		value.resize(112);

		QFloat rs;
		for (int i = 0; i < 4; i++)
			rs.m_arr[i] = 0;
		for (int i = 0; i < 112; i++) {
			if (value[111 - i] == '1')
				rs.setbit1(i);
		}
		if (this->getSign() != temp.getSign()) {
			rs.setbit1(127);
		}

		return rs;

	}

	if (newExp > 16384) {

	}
	value.erase(0, 1);
	value.resize(112);
	QFloat rs;
	for (int i = 0; i < 4; i++)
		rs.m_arr[i] = 0;
	for (int i = 0; i < 112; i++) {
		if (value[111 - i] == '1')
			rs.setbit1(i);
	}
	newExp += 16383;
	for (int i = 112; i < 127; i++) {
		if ((newExp >> (i - 112)) & 1 == 1)
			rs.setbit1(i);
	}
	if (this->getSign() != temp.getSign()) {
		rs.setbit1(127);
	}
	
	return rs;
}

QFloat & QFloat::operator=(const QFloat & other)
{
	if (this != &other) {
		for (int i = 0; i < 4; i++)
			m_arr[i] = other.m_arr[i];
	}

	return *this;
	// TODO: insert return statement here
}

int QFloat::operator[] (int i)// gia tri bit thu i
{
	if (i > 127 || i < 0) return 0;
	return getBit(i);
}

string QFloat::divideSignificand(string snf1, string snf2)
{
	QInt numInt1(snf1, 2), numInt2(snf2, 2);
	pair<QInt, QInt> q_r[8];
	q_r[0] = numInt1 / numInt2;
	return string();
}
