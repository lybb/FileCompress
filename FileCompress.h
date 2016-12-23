#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "HuffmanTree.h"
#include <string>
#include <Windows.h>
#include <cassert>

typedef long long LongType;
struct CharInfo
{
	CharInfo(LongType count=0)
		:_count(count)
	{}

	CharInfo operator+(const CharInfo& info)
	{
		return CharInfo(_count + info._count);
	}
	bool operator< (const CharInfo& info)
	{
		return _count < info._count;
	}
	bool operator !=(const CharInfo& info)
	{
		return _count != info._count;
	}


	unsigned char _ch;	//出现的字符
	LongType _count;	//字符出现的次数
	string _code;		//字符编码
};

class FileCompress
{
public:
	FileCompress()
	{
		for (size_t i=0; i<256; i++)
		{
			_info[i]._ch = i;
			_info[i]._count = 0;
		}
	}   
public:
	string Compress(const char* filename)		//文件压缩
	{
		FILE* fout = fopen(filename,"rb");
		assert(fout);
		//每次从文件中读取一个字符，统计各字符出现的次数
		unsigned char ch = fgetc(fout);
		while(!feof(fout))
		{
			_info[ch]._count ++;
			ch = fgetc(fout);
		}

		//用各字符的次数构建哈夫曼树
		CharInfo invalid;
		HuffmanTree<CharInfo> h(_info,256,invalid);

		//得到哈夫曼编码
		string code;
		GenerateHuffmanTreeCode(h.GetRoot(),code);
		//GenerateHuffmanTreeCode(h.GetRoot());

		//进行压缩
		string compressfile = filename;
		size_t index1 = compressfile.rfind('.');
		compressfile = compressfile.substr(0,index1);
		compressfile += ".huffman";	//先改变文件的后缀
		FILE* fin = fopen(compressfile.c_str(),"wb");//以写入的方式打开"compressfile.huffman"
		assert(fin);
		fseek(fout,0,SEEK_SET);	//设置文件指针的位置为文件开头
		int pos = 0;
		char value = 0;
		unsigned char c = fgetc(fout);
		//while (c != EOF)
		while(!feof(fout))
		{
			//将字符转为二进制位，满8位就写入
			string& code = _info[c]._code;
			for (size_t i=0; i<code.size(); ++i)
			{
				value <<= 1;
				if (code[i] == '1')	
					value |= 1;
				else
					value |= 0;
				++pos;	//标示8位二进制位
				if(pos == 8)	//满8位就将value写进压缩文件
				{
					fputc(value,fin);
					pos = 0;
					value = 0;
				}
			}
			c = fgetc(fout);	//读取下一个字符
		}
		if (pos)	//pos==0，说明刚好存完8位；pos!=0，说明还有多余的位
		{
			value <<= (8-pos);
			fputc(value,fin);
		}
		fclose(fin);
		fclose(fout);

		//配置文件---存储字符出现的次数
		string configfilename = filename;
		size_t index2 = configfilename.rfind('.');
		configfilename = configfilename.substr(0,index1);
		configfilename += ".config";
		FILE* foutConfig = fopen(configfilename.c_str(),"wb");
		string line;
		char buff[128];
		for(size_t i=0; i<256; ++i)
		{
			if (_info[i]._count)
			{
				line += _info[i]._ch;
				line += ',';
				line += _itoa(_info[i]._count,buff,10);
				line += '\n';
				fputs(line.c_str(),foutConfig);
				//fwrite(line.c_str(),1,line.size(),foutConfig);//视频和音频
				line.clear();
			}
		}
		fclose(foutConfig);
		return compressfile;
	}
	//读取文件的一行 
	bool ReadLine(FILE* configfile,string& line)
	{
		char ch = fgetc(configfile);
		if (ch == EOF)
			return false;
		while (ch != EOF && ch != '\n')
		{
			line += ch;
			ch = fgetc(configfile);
		}
		return true;
	}
	string UnCompress(const char* filename)
	{
		assert(filename);
		//读取配置文件---获取字符出现的次数
		string configfilename = filename;	//filename --- "file.huffman"	配置文件---"file.config"
		size_t index1 = configfilename.rfind('.');
		configfilename = configfilename.substr(0,index1);
		configfilename += ".config";
		FILE* foutConfig = fopen(configfilename.c_str(),"rb");
		assert(foutConfig);
		string line;
		unsigned char ch = 0;
		while (ReadLine(foutConfig,line))
		{
			if (line.empty())
			{
				line += '\n';
				continue;
			}
			else
			{
				ch = line[0];
				_info[ch]._count = atoi(line.substr(2).c_str());
				line.clear();
			}
		}
		//构建Huaaman树
		CharInfo invalid;
		HuffmanTree<CharInfo> h1(_info,256,invalid);
		//根节点的权值为字符出现的总和
		LongType charCount = h1.GetRoot()->_weight._count;
		//压缩文件
		string name = filename;
		size_t index = name.rfind('.');
		name = name.substr(0,index);
		name += ".huffman";
		FILE* fout = fopen(name.c_str(),"rb");
		assert(fout);
		//由压缩文件还原原文件
		string uncompressfilename = filename;
		size_t index2 = uncompressfilename.find('.');
		uncompressfilename = uncompressfilename.substr(0,index2);
		uncompressfilename += ".unhufman";
		FILE* foutuncompress = fopen(uncompressfilename.c_str(),"wb");
		assert(foutuncompress);

		int pos = 7;
		unsigned char c = fgetc(fout);
		HuffmanTreeNode<CharInfo>* root = h1.GetRoot();
		HuffmanTreeNode<CharInfo>* cur = root;
		//while (charCount && ch != EOF)
		while (charCount && !feof(fout))
		{
			//到叶子节点说明找到字符，放入解压文件
			if (cur->_left == NULL && cur->_right == NULL)
			{
				fputc(cur->_weight._ch,foutuncompress);
				charCount--;
				cur = root;
			} 
			//如果不是叶子结点就找叶子节点，0向左走，1向右走
			if (c & (1<<pos))
				cur = cur->_right;
			else
				cur = cur->_left;
			pos--;
			//当pos<0时，说明此时已经解压完一个字节，开始解压下一个字节
			if (pos < 0)
			{
				pos = 7;
				c = fgetc(fout);
			}
			//当charCount == 0时，字符已经全部解压完
			if (charCount == 0)
				break;
		}
		return uncompressfilename.c_str();
	}
protected:
	void GenerateHuffmanTreeCode(HuffmanTreeNode<CharInfo>* root,string& code)//得到哈夫曼编码
	{
		if(root == NULL)
			return ;


		GenerateHuffmanTreeCode(root->_left,code+'0');
		GenerateHuffmanTreeCode(root->_right,code+'1');

		if (root->_left == NULL && root->_right == NULL)//到叶子结点，说明编码完成
			_info[root->_weight._ch]._code = code;
		
		//先遍历左子树，再遍历右子树
		//if (root->_left)	//左为0
			//GenerateHuffmanTreeCode(root->_left,code+'0');
		//if (root->_right)		//右为1
			//GenerateHuffmanTreeCode(root->_right,code+'1');
	}

	void GenerateHuffmanTreeCode(HuffmanTreeNode<CharInfo>* root)
	{
		if(root == NULL)
			return ;

		//先遍历左子树，再遍历右子树
		GenerateHuffmanTreeCode(root->_left);
		GenerateHuffmanTreeCode(root->_right);

		if (root->_left == NULL && root->_right == NULL)//找到叶子节点，说明找到字符,生成哈夫曼编码
		{
			string code;
			HuffmanTreeNode<CharInfo>* cur = root;
			HuffmanTreeNode<CharInfo>* parent = cur->_parent;
			while (parent)
			{
				if (parent->_left == cur)	//左边就插入'0'
					//code.push_back('0');
					code += '0';
				else
					//code.push_back('1');	//右边就插入'1'
					code += '1';

				cur = parent;
				parent = cur->_parent;
			}
			reverse(code.begin(),code.end());
			_info[root->_weight._ch]._code = code;

		}
	}
protected:
	CharInfo _info[256];
};

void TestCompress()
{
	FileCompress filecompress;
	int start = GetTickCount();
	//filecompress.Compress("file.txt");
	filecompress.Compress("music.mp3");
	//filecompress.Compress("picture.jpg");
	//filecompress.Compress("Input.BIG");
	int end = GetTickCount();
	cout<<"Compress():"<<end-start<<endl;

	start = GetTickCount();
	//filecompress.UnCompress("file.huffman");
	filecompress.UnCompress("music.huffman");
	//filecompress.UnCompress("picture.huffman");
	//filecompress.UnCompress("Input.huffman");
	end = GetTickCount();
	cout<<"UnCompress:"<<end-start<<endl;
}