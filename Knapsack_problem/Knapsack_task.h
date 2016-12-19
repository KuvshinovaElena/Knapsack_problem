#pragma once
#include <vector>
#include "iofile.h"

using std::vector;

namespace Prep
{
	long multipl(long, long, long);
	long inverse(const long, const long, long * d);
}

namespace Knapsack_task
{
	vector<long> createPublicKey(vector<long>, long, long);
	bool encrypt(const char *,const vector<long> privkey, const char*, long, long);
	bool decrypt(const char*, const vector<long> privKey, const char *, long, long);
	long encryptBlock(const vector<bool> block, const vector<long> privKey);
	vector<bool> decryptBlock(const long block, const vector<long> privKey, long invN, long m);
}

long Prep::multipl(long a, long b, long mod)
{
	long res = (a*b) % mod;
	if (res == mod) res = 0;
	return res;
}

long Prep::inverse(const long a, const long n, long * d)
{
	long r1 = a, r2 = n;
	long x;
	long q, r, x1, x2;
	if (n == 0)	return 1;
	x2 = 1, x1 = 0;
	while (r2 > 0) {
		q = r1 / r2; 
		r = r1 - q * r2;
		x = x2 - q * x1;
		r1 = r2;
		r2 = r;
		x2 = x1;
		x1 = x;
	}
	x = x2;
	*d = r1;
	return x+n;
}

vector<long> Knapsack_task::createPublicKey(vector<long> privkey, long n, long m)
{
	vector<long> pubkey(privkey.size());

	for (size_t i = 0; i<privkey.size(); i++)
	{
		pubkey[i] = Prep::multipl(privkey[i], n, m);
	}
	return pubkey;
}

long Knapsack_task::encryptBlock(const vector<bool> block, const vector<long> pubKey)
{
	long result = 0;
	for (size_t i = 0; i < block.size(); i++)
	{
		result += block[i] * pubKey[i];
	}
	return result;
}

vector<bool> Knapsack_task::decryptBlock(const long block, const vector<long> privKey, long invN, long mod)
{
	long res = Prep::multipl(block, invN,mod);
	vector<long> knapsack;	//Рюкзак
	for (int i = privKey.size()-1; i >= 0; i--)
	{
		if (privKey[i] <= res)
		{
			knapsack.push_back(privKey[i]);
			res -= privKey[i];
		}
	}
	vector<bool> knapset(privKey.size());
	if (!knapsack.empty())
	{
		for (size_t i = 0; i < knapset.size(); i++)
		{
			if (std::find(knapsack.begin(), knapsack.end(), privKey[i]) != knapsack.end())
			{
				knapset[i] = 1;
			}
			else knapset[i] = 0;
		}
	}
	return knapset;
}

bool Knapsack_task::encrypt(const char * input, const vector<long> privkey, const char* outfile, long n, long mod)
{
	long test = 0;
	for (size_t i = 0; i < privkey.size(); i++)
	{
		if (test > privkey[i]) return 0;	//Секретный ключ должен быть сверхвозрастающей последовательностью
		test += privkey[i];
	}
	if (test > mod) return 0;	//Значение модуля должно быть больше суммы всех чисел последовательности
	Prep::inverse(n, mod, &test);
	if (test != 1) return 0;	//Модуль и множитель должны быть ваимно простыми числами
	vector<long> pubkey = createPublicKey(privkey, n, mod);
	ifstream in(input, std::ios::binary);
	ofstream out(outfile);
	if (!in.is_open() || !out.is_open()) return 0;
	int pos = static_cast<int>(in.tellg());
	while (pos != EOF)
	{
		vector<bool> block = FileIO::readBlockBit(in,pubkey.size()/8,pos);
		long code = encryptBlock(block,pubkey);
		FileIO::writeBlockLong(out, code);
	}
	in.close();
	out.close();
	return 1;
}

bool Knapsack_task::decrypt(const char* input, const vector<long> privKey, const char* output, long n, long mod)
{
	long test = 0;
	for (size_t i = 0; i < privKey.size(); i++)
	{
		if (test > privKey[i]) return 0;	//Секретный ключ должен быть сверхвозрастающей последовательностью
		test += privKey[i];
	}
	if (test > mod) return 0;	//Значение модуля должно быть больше суммы всех чисел последовательности
	long invN = Prep::inverse(n, mod, &test);
	if (test != 1) return 0;	//Модуль и множитель должны быть ваимно простыми числами
	ifstream in(input);
	ofstream out(output, std::ios::binary | std::ios::out);
	if (!in.is_open() || !out.is_open()) return 0;
	while (int(in.tellg()) != EOF)
	{
		long code = FileIO::readBlockLong(in);
		vector<bool> block = decryptBlock(code, privKey, invN, mod);
		FileIO::writeBlockBit(out,block);
	}
	in.close();
	out.close();
	return 1;
}

