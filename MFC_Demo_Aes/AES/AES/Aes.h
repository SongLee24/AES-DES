#pragma once
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#define Nr 10   // AES-128需要 10 轮加密
#define Nk 4    // Nk 表示输入密钥的 Word 个数
using namespace std; 
typedef bitset<8> Byte;
typedef bitset<32> Word;

class Aes
{
private:
	static Byte S_Box[16][16];         // S盒
	static Byte Inv_S_Box[16][16];     // 逆S盒
	static Byte Mul_02[256];           // GF(2^8)乘法表
	static Byte Mul_03[256];
	static Byte Mul_09[256];
	static Byte Mul_0b[256];
	static Byte Mul_0d[256];
	static Byte Mul_0e[256];
	static Word Rcon[10];              // 轮常数

public:
	void SubBytes(Byte mtx[]);               // S盒变换
	void ShiftRows(Byte mtx[]);              // 行变换
	void MixColumns(Byte mtx[]);             // 列变换
	void AddRoundKey(Byte mtx[], Word k[]);  // 轮密钥加变换

	void InvSubBytes(Byte mtx[]);            // 逆S盒变换
	void InvShiftRows(Byte mtx[]);           // 逆行变换
	void InvMixColumns(Byte mtx[]);          // 逆列变换

	Word word(Byte&, Byte&, Byte&, Byte&);   // 合并四个Byte为一个Word
	Word RotWord(Word& rw);                  // 按字节循环左移
	Word SubWord(Word& sw);                  // 对每个字节进行S-盒变换
	void KeyExpansion(Byte key[], Word w[]); // 密钥扩展函数

	void encrypt(Byte in[], Word w[]);       // 加密
	void decrypt(Byte in[], Word w[]);       // 解密

	void charToByte(Byte out[], const char s[]);
	void divideByte(Byte out[], bitset<128>&);
	bitset<128> mergeByte(Byte in[]);
};

