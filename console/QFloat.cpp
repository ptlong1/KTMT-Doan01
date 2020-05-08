#include "QFloat.h"
QFloat::QFloat(vector<int> vtBin) {
	for (int i = 0; i < vtBin.size(); i++)
		if (vtBin[i] == 1) setbit1(i);
		else setbit0(i);
}
QFloat::QFloat() {
	for (int i = 0; i < 4; i++)
		m_arr[i] = 0;
}
QFloat::QFloat(string str, int base) {
	vector<int> vtBin = convert2vt_bin(str, base);
	for (int i = 0; i < vtBin.size(); i++)
		if (vtBin[i] == 1) setbit1(i);
		else setbit0(i);
}
QFloat::~QFloat() {}
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
			vtBin.insert(vtBin.begin(),dec_val % 2);
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
	for (int i = str.size()-1; i >= 0; i--) {
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
int QFloat::getBit(int i) const{
	return ((m_arr[i / 32] >> (31 - i % 32)) & 1);
}
void QFloat::PrintQFloat(ostream& f,int base) {
	if (base == 2) {
		for (int i = 0; i < SIZE; i++) {
			f << getBit(i);
			if (i == 0 || i == 15) f << " ";
		}
		f << endl;
	}
	else if (base == 10) {
		double ans=0;
		long exp = 0;
		for(int i=0; i<SIGNIFICANT_SIZE; i++)
			ans += getBit(i+1+EXPONENT_SIZE)* pow(2, -i-1);
		ans += 1;
		for (int i = 0; i < EXPONENT_SIZE; i++)
			exp += getBit(1 + i)*pow(2, EXPONENT_SIZE - i - 1);
		exp -= pow(2, EXPONENT_SIZE - 1) - 1;
		f << ans * pow(2, exp) << endl;
	}
}

void QFloat::setbit0(int i) {
	m_arr[i / 32] = m_arr[i / 32] & (~(1 << (31 - i % 32)));
}

void QFloat::setbit1(int i) {
	m_arr[i / 32] = (m_arr[i / 32] | (1 << (31 - i % 32)));
}
bool QFloat::isZero() const{
	for (int i = 1; i < 4; i++)
		if (m_arr[i] != 0) return false;
	if (m_arr[0] != 0 && m_arr[0] != pow(2, 31)) return false; //bit dấu có thể là 0 hoặc 1
	return true;
}
vector<int> QFloat::get_exponent() const{
	vector<int> ans;
	for (int i = 0; i < EXPONENT_SIZE; i++)
		ans.push_back(getBit(i + 1));		//vì bit mũ bắt đầu từ bit số 1
	return ans;
}
vector<int> QFloat::get_snfcant() const {
	vector<int> ans;
	for (int i = 0; i < SIGNIFICANT_SIZE; i++)
		ans.push_back(getBit(1 + EXPONENT_SIZE+i));
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
	for (int i = 0; i < SIGNIFICANT_SIZE+1; i++) {
		if (ans.back() == 1) {
			//Cộng s1
			for (int j = SIGNIFICANT_SIZE + 1; j >= 1; j--) {
				sum = ans[j] + s1[j-1] + carry;
				carry = sum > 1 ? 1 : 0;
				ans[j] = sum % 2;
			}
			ans[0] += carry;
		}
		ans.pop_back();
		ans.insert(ans.begin(), 0);
	}
	while (ans.size()> 0 &&ans.front() == 0) ans.erase(ans.begin());
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
	for (int i = 0; i <vt.size(); i++)
		vt[i] =0;
}
vector<int> QFloat::add_exponent(vector<int> exp1, vector<int> exp2) {
	vector<int> ans;
	vector<int> bias;
	bias.assign(EXPONENT_SIZE, 1);
	bias[0] = 0;

	int carry = 0;
	int sum = 0;
	for (int i = EXPONENT_SIZE - 1; i >= 0; i--) {			//cộng hai phần dấu
		sum = exp1[i] + exp2[i] - bias[i] + carry ;
		if (sum > 1) carry = 1;
		else if (sum < 0) carry = -1;
		else carry = 0;
		ans.insert(ans.begin(), (sum+2) % 2);
	}
	
	if (exp1[0] == exp2[0] && exp1[0] != ans[0]) {				//nếu cùng dấu mà cộng ra số trái dấu
		if (exp1[0] == 0 && ans[0] == 1)			//âm+âm ra dương => underflow			
			ans.insert(ans.begin(), -2);	
		else if (exp1[0] == 1 && ans[0] == 0) ans.insert(ans.begin(), 2);	//dương+dương ra âm=>overflow
	}
	else if(check_all_bit0(ans))//cộng ra -2^(n-1)+1 vẫn là underflow 
		ans.insert(ans.begin(), -3);
	else ans.insert(ans.begin(), -1);				//không thuộc trường hợp đặc biệt
	return ans;
}
void QFloat::handle_multi_special_case(int flag, vector<int>& exp, vector<int>& snf)
{
	int exp_val = 0;
	for(int i=0; i<EXPONENT_SIZE; i++)
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
QFloat QFloat::operator*(const QFloat& other)
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
	vt_ans.assign(1, this->getBit(0)^other.getBit(0));			//cùng dấu thì ra dương, khác dấu ra âm
	vt_ans.insert(vt_ans.end(), exponent_ans.begin(), exponent_ans.end());
	vt_ans.insert(vt_ans.end(), snfcant_ans.begin(), snfcant_ans.end());
	QFloat ans(vt_ans);
	return ans;
}
