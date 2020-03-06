#ifndef __LZWNODE__H__
#define __LZWNODE__H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;
typedef unsigned char byts;

class LzwNode
{
public:
	int odddata;
	byts newdata;
};

class Lzw
{
public:
	int len;
	LzwNode lzw[4096];
	void unitLzw();
	void add(int ad, byts x);
	int matchDatas(byts *ss, int sscount, int i);
	int match(byts *ss, int sscount);
	int check(int odddata, byts newdata);
	byts getHead(int i);

};

class LzwCompression
{
public:
	void lzwCompression();
	void unlzwCompression();//解压缩方法
	void ShowUI();
};
#endif
