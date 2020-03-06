#include "lzwnode.h"

void Lzw::unitLzw()
{
	len = 0;
	for (int i=0; i<256; ++i){
		lzw[i].odddata = -1;
		lzw[i].newdata = i;
		cout << "[" << i << "]" << lzw[i].odddata << " " << (int)lzw[i].newdata << endl;
		len++;
	}
}

void Lzw::add(int ad, byts x)
{
	lzw[len].odddata = ad;
	lzw[len].newdata = x;
	len++;
}

//传入要对比的数组及在字典上的位置
int Lzw::matchDatas(byts *ss, int sscount, int i)
{
	if (i < 256){
		if (((int)lzw[i].newdata == (int)ss[sscount-1]) && (sscount == 1)){
			return 1;
		}else {
			return 0;
		}
	}else {
		if (((int)lzw[i].newdata == (int)ss[sscount-1]) && (sscount >= 1)){
			return matchDatas(ss, sscount-1, lzw[i].odddata);
		}else {
			return 0;
		}
	}
}

int Lzw::match(byts *ss, int sscount)
{
	for (int i=0; i<len; ++i){
		if (matchDatas(ss, sscount, i) == 1){
			return i;
		}
	}

	return -1;
}

int Lzw::check(int odddata, byts newdata)
{
	for (int i=0; i<len; ++i){
		if ((lzw[i].odddata == odddata) && ((int)lzw[i].newdata == (int)newdata)){
			return i;
		}
	}
	return -1;
}

byts Lzw::getHead(int i)
{
	if (i < 256){
		return lzw[i].newdata;
	}else{
		return getHead(lzw[i].odddata);
	}
}

void LzwCompression::lzwCompression()
{
	ifstream file1;
	ofstream file2;
	Lzw lz;
	string path1, path2;
	cout <<"请输入要压缩的文件名和地址" << endl;
	cin >> path1;
	cout << "请输入压缩后要保存的文件名和地址" << endl;
	cin >> path2;

	int fore, after;
	file1.open(path1.c_str(), ios::binary);
	file2.open(path2.c_str(), ios::binary);
	char x;
	int times = 0;
	int odd = 0; // 控制奇偶
	byts odd0, odd1, odd2;
	int mathint;
	byts s[4097];
	int scount;

	while (!file1.eof()){
		lz.unitLzw();
		char x;
		scount = 0;
		fore = -1;
		while (!file1.eof() && lz.len <= 4096){
			//在match中记录上次的地址，保存在address中
			file1.get(x);
			after = (int)(byts)x;
		}

		//当前串进LZW
		if (lz.check(fore, (byts)after) < 0){
			lz.add(fore, (byts)after);
		}

		//输出前缀
		if (odd == 0){
			odd0 = (byts)(fore >> 4);
			odd1 = (byts)(fore & 0x0000000F) << 4;
		}else {
			odd1 = odd1 + (byts)(fore >> 8);
			odd2 = (byts)(fore);
			file2.put(odd0);
			file2.put(odd1);
			file2.put(odd2);
		}
		odd = (odd+1) % 2;
		fore = after;
	}
	if (odd == 0){
		if (odd == 0){
			odd0 = (byts)(fore >> 4);
			odd1 = (byts)(fore & 0x0000000F) << 4;
		}else {
			odd1 = odd1 + (byts)(fore >> 8);
			odd2 = (byts)(fore);
			file2.put(odd0);
			file2.put(odd1);
			file2.put(odd2);
		}
		odd = (odd+1) % 2;
	}
	//还有一个FORE
	if (odd == 0){
		odd0 = (byts)(fore >> 4);
		odd1 = (byts)(fore & 0x0000000F) << 4;
	}else {
		odd1 = odd1 + (byts)(fore >> 8);
		odd2 = (byts)(fore);
		file2.put(odd0);
		file2.put(odd1);
		file2.put(odd2);
	}
	odd = (odd + 1) % 2;
	if (odd == 1){
		file2.put(odd0);
		file2.put(odd1);
	}

	file1.close();
	file2.close();
}

//解压缩方法

void LzwCompression::unlzwCompression()
{
	ifstream file1;
	ofstream file2;
	Lzw lz;
	string path1, path2;
	cout << "请输入要解压的文件的位置和名称" << endl;
	cin >> path1;
	cout << "请输入解压后要保存文件的地址和名称" << endl;
	cin >> path2;
	file1.open(path1.c_str(), ios::binary);
	file2.open(path2.c_str(), ios::binary);

	char x;
	int times = 0;
	int num1, num2;
	int fore, after;
	int odd = 0;
	byts odd0, odd1, odd2;
	int matchint;
	byts s[4097];
	int scount;
	int ii;
	
	while (!file1.eof()){
		lz.unitLzw();
		fore = -1;
		if (odd == 1){
			num1 = num2;
		}else{
			file1.get(x);
			odd0 = (byts)x;
			file1.get(x);
			odd1 = (byts)x;
			file1.get(x);
			odd2 = (byts)x;
			
			num1 = (((int)odd0 << 4) + ((int)odd1 >> 4));
			num2 = (((int)(odd1 & 0x0F)) << 8 + ((int)odd2));
		}
		odd = (odd+1)%2;
		scount = 0;
		matchint = -1;
		
		while (!file1.eof() && (lz.len <= 4096)){
			//若当前值不在字典中
			if (num1 >= lz.len){
				after = lz.getHead(fore);
				lz.add(fore, lz.getHead(fore));
				fore = num1;
			}else{
				after = num1;
				//当前串不在字典中
				if (lz.check(fore, lz.getHead(after)) < 0){
					lz.add(fore, lz.getHead(after));
				}
				fore = after;
			}

			//输入第几个位置，获得对应的字典数据
			ii = fore;
			scount = 0;
			cout << "进";
			cout << "到了 : " << " ' " << ii << " ' ";
			while (ii >= 256){
				s[scount++] = lz.lzw[ii].newdata;
				ii = lz.lzw[ii].odddata;
				cout << "到了 : " << " ' " << ii << " ' ";
			}
			cout << "出";
			s[scount++] = lz.lzw[ii].newdata;

			//输出
			for (int i=scount-1; i>=0; --i){
				file2.put(s[i]);
				cout << (int)s[i] ;
			}
			if (odd == 1){
				num1 = num2;
			}else {
				file1.get(x);
				odd0 = (byts)x;
				file1.get(x);
				odd1 = (byts)x;
				file1.get(x);
				odd2 = (byts)x;

				num1 = (((int)odd0 << 4) + ((int)odd1 >> 4));
				num2 = (((int)(odd1 & 0x0F) << 8) + ((int)odd2)); 
			}
			odd = (odd+1) %2;
		}
	}
	file1.close();
	file2.close();
}

void LzwCompression::ShowUI()
{int choice;
	printf("\n\n\n\n");
	printf("\n\n\n		*********LZW压缩软件*********\n\n\n");
	printf("			1.压缩文件\n\n");
	printf("			2.解压缩文件\n\n");
	printf("			0.退出软件\n\n");
	scanf("%d", &choice);
	if (choice < 1 && choice > 2){
		choice = 0;
	}
	switch (choice){
		case 1:
			lzwCompression();
			break;
		case 2:
			unlzwCompression();
			break;
		case 0:
		exit(0);
	}
}
