#pragma once
#include <vector>
#include <bitset>
using std::vector;
using std::bitset;

namespace Prep
{
	long multipl(long, long, long);
	long inverse(const long, const long);
}

namespace Knapsack_task
{
	vector<long> createPublicKey(vector<long>, long, long);
	void encrypt(const char *,const vector<long> privkey, const char* );
	void decrypt(const char*, const vector<long> privKey, const char *);
	long encryptBlock(const vector<bool> block, const vector<long> privKey);
	vector<bool> decryptBlock(const long block, const vector<long> privKey, long n, long m);
}

long Prep::multipl(long a, long b, long mod)
{
	long res = (a*b) % mod;
	if (res == mod) res = 0;
	return res;
}

long Prep::inverse(const long a, const long n)
{
	long r1 = a, r2 = n;
	long x;
	long q, r, x1, x2, y1, y2;
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
	return n+x;
}



vector<long> Knapsack_task::createPublicKey(vector<long> privkey, long n, long m)
{
	vector<long> pubkey(privkey.size());

	for (int i = 0; i<privkey.size(); i++)
	{
		pubkey[i] = Prep::multipl(privkey[i], n, m);
	}
	return pubkey;
}

long Knapsack_task::encryptBlock(const vector<bool> block, const vector<long> pubKey)
{
	long result = 0;
	for (int i = 0; i < block.size(); i++)
	{
		result += block[i] * pubKey[i];
	}
	return result;
}

vector<bool> Knapsack_task::decryptBlock(const long block, const vector<long> privKey, long n, long mod)
{
	long invN = Prep::inverse(n, mod);
	long res = Prep::multipl(block, invN,mod);
	vector<long> knapsack;	//аўъчръ
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
		for (int i = 0; i < knapset.size(); i++)
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

void Knapsack_task::encrypt(const char * input, const vector<long> privkey, const char* outfile)
{
	vector<long> pubkey = createPublicKey(privkey, 31, 105);
	vector<bool> block({ 0,1,1,0,0,0 });
	long code = encryptBlock(block,pubkey);
	return;
}

void Knapsack_task::decrypt(const char* input, const vector<long> privKey, const char* output)
{
	long bl = 174;
	vector<bool> block;
	block = decryptBlock(bl,privKey,31,105);
	return;
}