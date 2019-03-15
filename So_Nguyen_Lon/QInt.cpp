#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include "QInt.h"

using namespace std;
string DecStrToBinStr(string x)
{
	string Result, Temp, p; //p thuong
	char buffer[10];
	while (x != "0")
	{
		Temp.clear();
		p.clear();
		Result = _itoa((x[x.length() - 1] - '0') % 2, buffer, 10) + Result;
		for (int i = 0; i < x.length(); i++)
		{
			Temp += x[i];
			p += _itoa(stoi(Temp) / 2, buffer, 10);
			Temp = _itoa(stoi(Temp) % 2, buffer, 10);
		}
		while (p[0] == '0' && p.length() != 1)
		{
			p.erase(0, 1);
		}
		x = p;
	}
	return Result;
}
int BinDec(string x)
{
	int s = 0, n = x.length();
	for (int i = 0; i < n; i++)
		s += (x[i]-'0')*pow(2, n - 1 - i);
	return s;
}
QInt::QInt()
{
	for (int i = 0; i < 4; i++)
		data[i] = 0;
}

QInt::QInt(string x)
{
	bool Temp;
	if (x[0] == '-')
	{
		Temp = true;
		x.erase(0, 1);
	}
	else
		Temp = false;
	
	(*this)=this->BinToQInt(DecStrToBinStr(x));
	if (Temp)
	{
		(*this) = ~(*this)+QInt("1");
	}
}
QInt::~QInt()
{
}

QInt QInt::operator+(const QInt & N)
{
	QInt Result;
	unsigned int Temp;
	for (int i = 3; i >=0; i--)
	{
		Temp = Result.data[i];
		Result.data[i] += this->data[i] + N.data[i];
		if ((Result.data[i] < this->data[i]|| Result.data[i] < N.data[i]||(this->data[i]==N.data[i]&&Temp==1&&Result.data[i]== Temp + this->data[i] + N.data[i]))&& i!=0)
			Result.data[i - 1]++;
	}
	return Result;
}

QInt QInt::operator-(const QInt & N)
{
	QInt Temp=N;
	Temp = ~Temp + QInt("1");
	return (*this)+Temp;
}

QInt QInt::operator/(const QInt & N)
{
	QInt A;
	QInt src = *this;
	if (src > QInt("0"))
		A = QInt("0");
	else
		A = QInt("-1");

	int k = 128;

	while (k > 0) {
		A = A << 1;
		int temp = (src.data[0] >> 31) & 1;
		src = src << 1;
		if (temp == 1)
			A = A + QInt("1");


		A = A - N;

		if (A < QInt("0")) {
			//src = src & QInt("0"); 110'1' -> 110'0'
			//src = src & ~QInt("1");
			A = A + N;
		}
		else
			src = src + QInt("1");

		k--;
	}
	return src;
}

QInt QInt::operator*(const QInt & N)
{
	QInt X=N, Y;
	bool Temp = false;
	if (X < QInt("0"))
	{
		X = QInt("0") - X;
		Temp = true;
	}
	QInt Result; // 1011 * 101
	int count = 0;
	while (X != QInt("0"))
	{
		if (X.data[3] % 2 == 1)
			Y = (*this);
		else
			Y = QInt("0");
		X = X >> 1;
		Y = Y << count;
		Result = Result + Y;
		count++;
	}
	if (Temp)
		Result = QInt("0") - Result;
	return Result;
}
QInt QInt::BinToQInt(string x)
{
	QInt Result; 
	string Temp;
	int count = 0;
	for (int i = 0; i < 4; i++)
		data[i] = 0;
	while (x.length() >= 32 && count < 4)
	{
		Temp = x.substr(x.length() - 32);
		x.erase(x.length() - 32);
		Result.data[3 - count] = BinDec(Temp) + Result.data[3 - count];
		count++;
	}
	Result.data[3 - count] = BinDec(x) + Result.data[3 - count];
	return Result;
}

QInt QInt::operator&(const QInt & N)
{
	QInt X;
	int i;
	for (i = 0; i < 4; i++)
		X.data[i] = this->data[i] & N.data[i];
	return X;
}

QInt QInt::operator|(const QInt & N)
{
	QInt X;
	int i;
	for (i = 0; i < 4; i++)
		X.data[i] = this->data[i] | N.data[i];
	return X;
}

QInt QInt::operator^(const QInt & N)
{
	QInt X;
	int i;
	for (i = 0; i < 4; i++)
		X.data[i] = this->data[i] ^ N.data[i];
	return X;
}

QInt QInt::operator~()
{
	QInt X;
	X = (*this);
	int i,j;
	for (i = 0; i < 4; i++)
	{
		for (j = 31; j >= 0; j--)
			X.data[i] = X.data[i] ^ (1 << (32 - j - 1));
		
	}
	return X;
}

QInt QInt::operator<<(int x)
{
	QInt Result;
	bool Temp=false;
	Result = (*this);
	if (Result.data[0] >= pow(2, 31))
	{
		Temp = true;
		Result.data[0] -= pow(2, 31);
	}
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (Result.data[j] >= pow(2, 31))
			{
				Result.data[j] -= pow(2, 31);
				if (j != 0)
					Result.data[j - 1] += 1;
			}
			Result.data[j] <<= 1;
		}
	}
	if (Temp&&Result.data[0] < pow(2, 31))
		Result.data[0] += pow(2, 31);
	return Result;
}

QInt QInt::operator>>(int x)
{
	QInt Result;
	bool Temp=false;
	Result = (*this);
	if (Result.data[0] >= pow(2, 31))
	{
		Temp = true;
		Result.data[0] -= pow(2, 31);
	}
	for (int i = 0; i < x; i++)
	{
		for (int j = 3; j >= 0; j--)
		{
			if (Result.data[j] %2==1)
			{
				if (j != 3)
					Result.data[j +1] += pow(2,31);
			}
			Result.data[j] >>= 1;
			
		}
		if (Temp)
			Result.data[0] += pow(2, 30);
	}
	if(Temp)
		Result.data[0] += pow(2, 31);
	return Result;
}

QInt QInt::rol(int x)
{
	QInt Result;
	bool Temp=false,Temp1;
	Result = (*this);
	if (Result.data[0] >= pow(2, 31))
	{
		Temp = true;
		Result.data[0] -= pow(2, 31);
	}
	for (int i = 0; i < x; i++)
	{
		Temp = false;
		for (int j = 0; j < 4; j++)
		{
			if (Result.data[j] >= pow(2, 31))
			{
				Result.data[j] -= pow(2, 31);
				if (j != 0)
					Result.data[j - 1] += 1;
			}
			Result.data[j] <<= 1;
		}
		if (Result.data[0] >= pow(2, 31))
			Result.data[3]++;
	}
	if (Temp&&Result.data[0] < pow(2, 31))
		Result.data[0] += pow(2, 31);
	return Result;
}
QInt QInt::ror(int x)
{
	QInt Result;
	bool Temp = false,Temp1;
	Result = (*this);
	if (Result.data[0] >= pow(2, 31))
	{
		Temp = true;
		Result.data[0] -= pow(2, 31);
	}
	for (int i = 0; i < x; i++)
	{
		Temp1 = false;
		for (int j = 3; j >= 0; j--)
		{
			if (Result.data[j] % 2 == 1)
			{
				if (j != 3)
					Result.data[j + 1] += pow(2, 31);
				else
					Temp1 = true;
			}
			Result.data[j] >>= 1;
		}
		if (Temp1)
			Result.data[0] += pow(2, 30);
	}
	if (Temp&&Result.data[0] < pow(2, 31))
		Result.data[0] += pow(2, 31);
	return Result;
}

QInt QInt::operator=(const QInt & N)
{
	for (int i = 0; i < 4; i++)
	{
		(*this).data[i] = N.data[i];                                                                                                                                                                                                                                                                 
	}
	return(*this);
}

int ctoi(char x)
{
	return x - '0';
}

bool QInt::operator>(const QInt & N)
{ 
	int SignA = this->data[0] >> 31 & 1;
	int SignB = N.data[0] >> 31 & 1;
	if (SignA < SignB)
		return true;
	if (SignA > SignB)
		return false;
	for (int i = 0; i < 4; i++)
	{
			if ((*this).data[i] < N.data[i])
			{
				if ((*this).data[i] <= N.data[i])
					return false;
			}
		
	}
	return true;
}

bool QInt::operator<(const QInt & N)
{
	int SignA = this->data[0] >> 31 & 1;
	int SignB = N.data[0] >> 31 & 1;
	if (SignA > SignB)
		return true;
	if (SignA < SignB)
		return false;
	for (int i = 0; i < 4; i++)
	{
		if ((*this).data[i] > N.data[i])
		{
			if ((*this).data[i] >= N.data[i])
				return false;
		}
	}
	return true;
}

bool QInt::operator>=(const QInt & N)
{
	int SignA = this->data[0] >> 31 & 1;
	int SignB = N.data[0] >> 31 & 1;
	if (SignA < SignB)
		return true;
	if (SignA > SignB)
		return false;
	for (int i = 0; i < 4; i++)
	{
		if ((*this).data[i] < N.data[i])
		{
				return false;
		}
	}
	return true;
}	

bool QInt::operator<=(const QInt & N)
{
	int SignA = this->data[0] >> 31 & 1;
	int SignB = N.data[0] >> 31 & 1;
	if (SignA > SignB)
		return true;
	if (SignA < SignB)
		return false;
	for (int i = 0; i < 4; i++)
	{
		if ((*this).data[i] > N.data[i])
		{
				return false;
		}
	}
	return true;
}

bool QInt::operator==(const QInt & N)
{
	int SignA = this->data[0] >> 31 & 1;
	int SignB = N.data[0] >> 31 & 1;
	if (SignA != SignB)
		return false;
	for (int i = 0; i < 4; i++)
	{
		if ((*this).data[i] != N.data[i])
		{
			return false;
		}
	}
	return true;
}

bool QInt::operator!=(const QInt & N)
{
	int SignA = this->data[0] >> 31 & 1;
	int SignB = N.data[0] >> 31 & 1;
	if (SignA != SignB)
		return true;
	for (int i = 0; i < 4; i++)
	{
		if ((*this).data[i] != N.data[i])
		{
			return true;
		}
	}
	return false;
}

string SumString(const string &a, const string &b)
{
	string ans;
	int sum, n = b.size(), count;

	if (a.size() < b.size())
	{
		n=a.size();
	}
	for (int i = 0; i < n; i++)
	{
		sum = ctoi(a[a.size() - i]) + ctoi(b[b.size() - i]);
		if (sum > 10)
		{
			count = sum % 10;
			sum /= 10;
		}
	}
	return string();
}

string QIntToString(const QInt &N)
{
	return string();
}
	


