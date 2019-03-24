﻿#include "Qfloat.h"

Qfloat::Qfloat()
{
	for (int i = 0; i < 4; i++)
		this->data[i] = 0;
}

Qfloat::Qfloat(string x, int mode)
{
	if (mode == 10)
	{

		char sign = '0';
		if (x[0] == '-')
		{
			sign = '1';
			x.erase(0, 1);
		}
		string phannguyen, phanthuc = "0";
		int i = 0;
		for (i; i < x.length(); i++)
		{
			if (x[i] == '.')// i sẽ tăng thêm 1 lần trc khi break
			{
				i++;
				break;
			}
			phannguyen += x[i];
		}
		for (i; i < x.length(); i++)
			phanthuc += x[i];// phần thực lưu trữ dạng 0123456789 thay vì 0.123456789
		while (phanthuc[phanthuc.length() - 1] == '0'&& phanthuc.length() > 1)//01230 --> 0123, 0 --> 0
			phanthuc.erase(phanthuc.length() - 1, 1);

		string bin_nguyen = DecStrToBinStr(phannguyen);
		string bin_thuc;
		if (phanthuc != "0")
		{
			int count = 0;
			do {
				phanthuc = Multiply(phanthuc, "2");
				while (phanthuc[phanthuc.length() - 1] == '0'&& phanthuc.length() > 1)
					phanthuc.erase(phanthuc.length() - 1, 1);

				if (phanthuc[0] == '0')
					bin_thuc += '0';
				else {
					bin_thuc += '1';
					if (phanthuc == "1")
						break;
					phanthuc[0] = '0';
				}
				count++;
			} while (phanthuc != "1" && count <= 112);
		}

		int exponent = 0;
		if (bin_nguyen.length() >= 2)
		{
			string temp;
			exponent = bin_nguyen.length() - 1;
			temp.append(bin_nguyen, 1, bin_nguyen.length() - 1);
			bin_thuc.insert(0, temp.c_str());
		}
		else// bin nguyen length ==1
		{
			if (bin_nguyen == "0")// về hình thức là dịch dấu phẩy sang phải
			{
				for (int i = 0; i < bin_thuc.length(); i++)
					if (bin_thuc[i] == '1')
					{
						exponent = i + 1;
						break;
					}
				bin_thuc.erase(0, exponent);// xóa 'exponent' số đầu tiên
				exponent *= -1;// có dạng như: 2^-2, 2^-100..
			}
		}
		bin_nguyen.clear();// bin nguyen hết tác dụng

		if (bin_thuc.length() < 112)
			bin_thuc.insert(bin_thuc.end(), 112 - bin_thuc.length(), '0');
		else // dài quá 112 thì cắt bớt phía bên phải
		{
			if (bin_thuc[111] == '0'&& bin_thuc[112] == '1')
				bin_thuc[111] == '1'; // làm tròn lên
			bin_thuc.resize(112);
		}
		string expo = Tra2(exponent + 16383);// quá k=16383
		if (expo.length() < 15)
			expo.insert(0, 15 - expo.length(), '0');
		string kq = sign + expo + bin_thuc;
		kq += sign;
		kq+= ' ' + expo + ' ' + bin_thuc;
		//cout << kq << endl;
		for (int i = 0, j = 0; i < 4; i++, j += 32)
			this->data[i] = BinDec(kq.substr(j, 32));
	}
	else if (mode == 2)
	{
		// mặc định x truyền vào phải đủ 128 bit
		this->BinToDec(x);
	}
}

Qfloat::Qfloat(const Qfloat & src)
{
	for (int i = 0; i < 4; i++)
		this->data[i] = src.data[i];
}

void Qfloat::ScanQfloat()
{
	string x;
	//char sign = '0';
	cout << "Enter a real number:";
	cin >> x;
	Qfloat temp(x, 10);
	(*this) = temp;

}

void Qfloat::PrintQfloat()
{
	string bit;
	for (int i = 0; i < 4; i++)
		bit += Tra2(this->data[i]);

	bool negative = false;
	if (bit[0] == '1')
		negative = true;
	bit.erase(0, 1);

	string exponent = bit.substr(0, 15);
	bit.erase(0, 15);
	// tới đây 'bit' chỉ chứa phần định trị
	int E = BinDec(exponent) - 16383;
	exponent.clear();// exponent hết tác dụng
	//cout << E << endl;
	while (bit[bit.length() - 1] == '0'&& bit.length()>1)
	{
		bit.erase(bit.length() - 1, 1);
	}//101101110000000 --> 10110111

	string bin_nguyen = "1";
	if (E > 0)
	{
		bin_nguyen.insert(bin_nguyen.length(), bit.substr(0, E));
		bit.erase(0, E);
	}
	else
	{
		bin_nguyen = "0";
		bit = '1' + bit;
		bit.insert(0, abs(E) - 1, '0');
	}
	cout << bin_nguyen<<"."<< bit<<endl;

	string kq_nguyen = "0";
	for (int i = bin_nguyen.length() - 1, j = 0; i >= 0; i--, j++)
	{
		if (bin_nguyen[i] == '1')
			kq_nguyen = Sum(kq_nguyen, Exponential("2", j));
	}
	string kq_thapphan;
	for (int i = 0; i < bit.length(); i++)
	{
		if (bit[i] == '1')
			kq_thapphan = Sum(kq_thapphan, HaiMuN_Am((i + 1)*-1));
	}
	kq_thapphan.erase(0, 1);/// 0.12345 thì còn .12345 để ráp vào kq_nguyên
	string kq = kq_nguyen + kq_thapphan;

	cout << kq << endl;
}

Qfloat Qfloat::BinToDec(string bit)
{
	// trong qfloat bit phải mặc định dài 128 bit
	for (int i = 0, j = 0; i < 4; i++, j += 32)
		this->data[i] = BinDec(bit.substr(j, 32));
	return Qfloat(*this);
}

string Qfloat::DecToBin()
{
	string bit;
	for (int i = 0; i < 4; i++)
		bit += Tra2(this->data[i]);
	return bit;
}

Qfloat & Qfloat::operator=(const Qfloat & src)
{
	if (this != &src)
	{
		for (int i = 0; i < 4; i++)
			this->data[i] = src.data[i];
	}
	return *this;
}

Qfloat::~Qfloat()
{
}
