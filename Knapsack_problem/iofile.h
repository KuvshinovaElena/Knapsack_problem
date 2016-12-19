#pragma once
#include<vector>
#include<bitset>
#include<fstream>
#include<iostream>

using std::vector;
using std::bitset;
using std::ifstream;
using std::ofstream;

namespace FileIO
{
	vector<bool> readBlockBit(ifstream& in, int sizeBlock, int& pos);
	long readBlockLong(ifstream& in);
	void writeBlockBit(ofstream& out, vector<bool> block);
	void writeBlockLong(ofstream& out, long n);
	int filedist(ifstream& f, int pos);
}

int FileIO::filedist(ifstream& f, int pos)
{
	int cur = static_cast<int>(f.tellg());
	f.seekg(0, std::ios::end);
	int fsize = static_cast<int>(f.tellg());
	f.seekg(std::streamoff(pos), std::ios::beg);
	return fsize - cur;
}

vector<bool> FileIO::readBlockBit(ifstream& in, int sizeBlock, int& pos)
{
	//sizeBlock == число байт
	bitset<8> block;
	try {
		pos = static_cast<int>(in.tellg());
	}
	catch (ifstream::failure e)
	{
		in.open("input.txt", std::ios::binary);
		pos = static_cast<int>(in.tellg());
	}
	int dist = filedist(in, pos);
	if (dist >= sizeBlock && pos>=0)
	{
		in.read(reinterpret_cast<char*>(&block), sizeBlock);
	}
	else
	{
		in.read(reinterpret_cast<char*>(&block), dist);
		for (int i = pos * 8; i<8 * sizeBlock; i++)
		{
			block[pos * 8] = 0;
		}
		pos = EOF;
	}
	vector<bool> bblock(8*sizeBlock);
	for (size_t i = 0; i < bblock.size(); i++)
	{
		bblock[i] = block[i];
	}
	return bblock;
}

long FileIO::readBlockLong(ifstream& in)
{
	if (!in.is_open()) in.open("output.txt", std::ios::binary);
	long code;
	in >> code;
	return code;
}

void FileIO::writeBlockBit(ofstream& out, vector<bool> block)
{
	if (!out.is_open()) out.open("decrypt.txt", std::ios::binary);
	bitset<8> bitblock;
	for (size_t i = 0; i < block.size(); i++)
	{
		bitblock[i] = block[i];
	}
	out.write(reinterpret_cast<char*>(&bitblock), 1);
}

void FileIO::writeBlockLong(ofstream& out, long n)
{
	if (!out.is_open()) out.open("output.txt", std::ios::binary);
	out << n<<" ";
}