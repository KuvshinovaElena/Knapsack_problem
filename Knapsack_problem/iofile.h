#pragma once
#include<vector>
#include<fstream>
#include<iostream>

using std::vector;
using std::ifstream;
using std::ofstream;

namespace FileIO
{
	vector<bool> readBlockBit(ifstream in, int sizeBlock);
	long readBlockLong(ifstream in);
	int writeBlockBit(ifstream out, vector<bool> block);
	int writeBlockLong(ifstream out, long n);
}

vector<bool> FileIO::readBlockBit(ifstream in, int sizeBlock)
{
	vector<bool> block(sizeBlock);
	int count = 0;
	int pos;
	try {
		pos = static_cast<int>(in.tellg());
	}
	catch (ifstream::failure e)
	{
		in.open("input.txt", std::ios::binary);
		pos = static_cast<int>(in.tellg());
	}
	while (pos != EOF)
	{
		in.read(reinterpret_cast<char*>(&block), sizeBlock);
		count++;
		pos = static_cast<int>(in.tellg());
		if (pos < 0)
		{
			in.clear();
			pos = static_cast<int>(in.tellg());
			pos = pos * sizeBlock - (count - 1) * 64 - 1;
			if (pos < 0) break;
			for (int i = pos; i < 64 && i>0; i++)
			{
				block[i] = 0;
			}
			pos = EOF;
		}
	}
	return block;
}