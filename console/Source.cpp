#pragma once
#include <fstream>
#include "QInt.h"
#include "QFloat.h"
#include <iomanip>
#include <string>

long long str2int(string str) {
	long long ans = 0;
	for (auto s : str) ans = ans * 10 + s - '0';
	return ans;
}


string calculateQInt(vector<string> operands, string operatr, int src_base, int des_base)
{
	/*
	Thực hiện tính toán khi đã có toán tử và toán hạng với kiểu QInt
	*/
	string ans;
	QInt a(operands[0], src_base);
	
	if (operatr.empty()) ans = a.toString(des_base);					//chuyển đổi giữa hai base
	else if (operatr == "~") ans = (~a).toString(src_base);				//phép đảo bit
	else if (operatr == "<<") {										//các phép toán mà operand thứ hai là số nguyên
		long long b = str2int(operands[1]);
		ans = (a << b).toString(src_base);
	}
	else if (operatr == ">>") {
		long long b = str2int(operands[1]);
		ans = (a >> b).toString(src_base);
	}
	else if (operatr == "ror") {
		long long b = str2int(operands[1]) % 128;
		ans = a.ror(b).toString(src_base);
	}
	else if (operatr == "rol") {
		long long b = str2int(operands[1]) % 128;
		ans = a.rol(b).toString(src_base);
	}
	else {		//các phép toán còn lại
		QInt b(operands[1], src_base);
		switch (operatr[0]) {
			case '+': ans = (a + b).toString(src_base); break;
			case '-': ans = (a - b).toString(src_base); break;
			case '*': ans = (a * b).toString(src_base); break;
			case '/': ans = (a / b).first.toString(src_base); break;
			case '&': ans = (a & b).toString(src_base); break;
			case '|': ans = (a | b).toString(src_base); break;
			case '^': ans = (a ^ b).toString(src_base); break;
			case '>': ans = (a > b)+'0'; break;
			case '<': ans = (a < b)+'0'; break;
			case '=': ans = (a == b)+'0'; break;
		}
		if (operatr == ">=") ans = (a >= b) + '0';
		else if (operatr == "<=") ans = (a <= b) + '0';
	}
	return ans;
}

string calculateQFloat(vector<string> operands, string operatr, int src_base, int des_base)
{
	/*
	Thực hiện tính toán khi đã có toán tử và toán hạng với kiểu QFloat
	*/
	string ans;
	QFloat a(operands[0], src_base);


	if (operatr.empty()) ans = a.toString(des_base);					//chuyển đổi giữa hai base
	else if (operatr == "<<") {										//các phép toán mà operand thứ hai là số nguyên
		int b = str2int(operands[1]) % 128;
		ans = (a << b).toString(src_base);
	}
	else if (operatr == ">>") {
		int b = str2int(operands[1]) % 128;
		ans = (a >> b).toString(src_base);
	}
	else {		//các phép toán còn lại
		QFloat b(operands[1], src_base);
		switch (operatr[0]) {
		case '+': ans = (a + b).toString(src_base); break;
		case '-': ans = (a - b).toString(src_base); break;
		case '*': ans = (a * b).toString(src_base); break;
		case '/': ans = (a / b).toString(src_base); break;
		}
	}
	return ans;
}


string handle_line(string line, int type) {
	/*
	Xử lí 1 dòng trong infile
	*/
	map<string, int> mBase = { {"2",2}, {"10",10}, {"16",16} };
	int src_base, des_base;		
	string operatr;				//toán tử
	vector<string> operands;	//các toán hạng

	//xác định là thực hiện phép toán hay chuyển đổi cơ số
	bool is_convert = line.find_first_of("+*/><=&|^~>><<rorrol") == string::npos;	//nếu không xuất hiện các phép toán tức là thực hiện chuyển đổi giữa hai base
	vector<int> minus_pos;					//vì dấu trừ có thể xuất hiện ở số âm 
	minus_pos.push_back(line.find('-'));
	while (minus_pos.back() != -1)	minus_pos.push_back(line.find('-', minus_pos.back() + 1));//lấy tất cả vị trí xuất hiện dấu trừ
	for (int i = 0; i < minus_pos.size(); i++)
		if (minus_pos[i] != -1 && line[minus_pos[i] - 1] == ' ' && line[minus_pos[i] + 1] == ' ') is_convert = false;	//nếu tồn tại 1 vị trí mà trước và sau dấu trừ đó là khoảng trắng thì nó là phép toán

	//tách các thành phần trong dòng (cách nhau bằng khoảng trắng)
	vector<string> para;	//các thành phần trong dòng
	int last_found = -1;
	size_t found = line.find(' ');
	do {		
		para.push_back(line.substr(last_found + 1, found - last_found -1 ));
		last_found = found;
		found = line.find(' ', last_found + 1);
	} while (last_found != string::npos);

	//tạo operand và operator
	src_base = mBase[para[0]];		//base đầu tiên luôn có
	des_base = mBase[para[1]];		
	if (!is_convert) {					// thực hiện phép toán trên 1 base, line có dạng <base> <operand> <operator> <operand> (trừ trường hợp phép ~)
		if (para[1] == "~") {			//riêng phép đảo bit thì operator nằm đằng trước operand
			operatr = para[1];
			operands.push_back(para[2]);
		}
		else {						//các phép toán còn lại thì operatr nằm giữa hai operand
			operatr = para[2];
			operands.push_back(para[1]);
			operands.push_back(para[3]);
		}
	}
	else operands.push_back(para[2]); //thực hiện chuyển đổi giữa hai base, không có operator
	
	//thực hiện tính toán
	if(type==1) return calculateQInt(operands, operatr, src_base, des_base);
	else return calculateQFloat(operands, operatr, src_base, des_base);
}

void execute(ifstream& infile, ofstream& outfile, int type) {
	/*
	Thực hiện đọc từng dòng infile, xử lí và ghi vào outfile
	*/
	string line;
	vector<string> lines;
	while (getline(infile, line)) lines.push_back(line);
	for (auto line : lines) outfile << handle_line(line, type) << endl;
}

int main(int argc, char* argv[]) {
	ifstream infile;
	ofstream outfile;
	int type = (argv[3][0]-'0');
	infile.open(argv[1]);
	outfile.open(argv[2]);
	if (!infile | !outfile) return 0;
	execute(infile, outfile, type);
	return 0;
}
