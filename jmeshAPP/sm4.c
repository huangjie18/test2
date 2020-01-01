#include <string.h>
#include "sm4.h"

static unsigned char Sbox[] = {
	0xD6, 0x90, 0xE9, 0xFE, 0xCC, 0xE1, 0x3D, 0xB7, 0x16, 0xB6, 0x14, 0xC2, 0x28, 0xFB, 0x2C, 0x05, 
	0x2B, 0x67, 0x9A, 0x76, 0x2A, 0xBE, 0x04, 0xC3, 0xAA, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99, 
	0x9C, 0x42, 0x50, 0xF4, 0x91, 0xEF, 0x98, 0x7A, 0x33, 0x54, 0x0B, 0x43, 0xED, 0xCF, 0xAC, 0x62, 
	0xE4, 0xB3, 0x1C, 0xA9, 0xC9, 0x08, 0xE8, 0x95, 0x80, 0xDF, 0x94, 0xFA, 0x75, 0x8F, 0x3F, 0xA6, 
	0x47, 0x07, 0xA7, 0xFC, 0xF3, 0x73, 0x17, 0xBA, 0x83, 0x59, 0x3C, 0x19, 0xE6, 0x85, 0x4F, 0xA8, 
	0x68, 0x6B, 0x81, 0xB2, 0x71, 0x64, 0xDA, 0x8B, 0xF8, 0xEB, 0x0F, 0x4B, 0x70, 0x56, 0x9D, 0x35, 
	0x1E, 0x24, 0x0E, 0x5E, 0x63, 0x58, 0xD1, 0xA2, 0x25, 0x22, 0x7C, 0x3B, 0x01, 0x21, 0x78, 0x87, 
	0xD4, 0x00, 0x46, 0x57, 0x9F, 0xD3, 0x27, 0x52, 0x4C, 0x36, 0x02, 0xE7, 0xA0, 0xC4, 0xC8, 0x9E, 
	0xEA, 0xBF, 0x8A, 0xD2, 0x40, 0xC7, 0x38, 0xB5, 0xA3, 0xF7, 0xF2, 0xCE, 0xF9, 0x61, 0x15, 0xA1, 
	0xE0, 0xAE, 0x5D, 0xA4, 0x9B, 0x34, 0x1A, 0x55, 0xAD, 0x93, 0x32, 0x30, 0xF5, 0x8C, 0xB1, 0xE3, 
	0x1D, 0xF6, 0xE2, 0x2E, 0x82, 0x66, 0xCA, 0x60, 0xC0, 0x29, 0x23, 0xAB, 0x0D, 0x53, 0x4E, 0x6F, 
	0xD5, 0xDB, 0x37, 0x45, 0xDE, 0xFD, 0x8E, 0x2F, 0x03, 0xFF, 0x6A, 0x72, 0x6D, 0x6C, 0x5B, 0x51, 
	0x8D, 0x1B, 0xAF, 0x92, 0xBB, 0xDD, 0xBC, 0x7F, 0x11, 0xD9, 0x5C, 0x41, 0x1F, 0x10, 0x5A, 0xD8, 
	0x0A, 0xC1, 0x31, 0x88, 0xA5, 0xCD, 0x7B, 0xBD, 0x2D, 0x74, 0xD0, 0x12, 0xB8, 0xE5, 0xB4, 0xB0, 
	0x89, 0x69, 0x97, 0x4A, 0x0C, 0x96, 0x77, 0x7E, 0x65, 0xB9, 0xF1, 0x09, 0xC5, 0x6E, 0xC6, 0x84, 
	0x18, 0xF0, 0x7D, 0xEC, 0x3A, 0xDC, 0x4D, 0x20, 0x79, 0xEE, 0x5F, 0x3E, 0xD7, 0xCB, 0x39, 0x48
};

static unsigned long CK[] = { 
	0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269, 0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
	0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249, 0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
	0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229, 0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299, 
	0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209, 0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279 
};

static unsigned long FK[] = { 0xa3b1bac6, 0x56aa3350, 0x677d9197, 0xb27022dc };

static unsigned long Rotl(unsigned long a, int b)
{
	return (((a) << (b)) | ((a) >> (32 - (b))));
}

static unsigned long ByteSub(unsigned long A)
{
	return (((unsigned long)Sbox[(A >> 24) & 0xFF] << 24) | ((unsigned long)Sbox[A >> 16 & 0xFF] << 16)
			| ((unsigned long)Sbox[(A >> 8) & 0xFF] << 8) | Sbox[A & 0xFF]);
}

static unsigned long L1(unsigned long B)
{
	return B ^ Rotl(B, 2) ^ Rotl(B, 10) ^ Rotl(B, 18) ^ Rotl(B, 24);
	// return B^(B<<2|B>>>30)^(B<<10|B>>>22)^(B<<18|B>>>14)^(B<<24|B>>>8);
}

static unsigned long L2(unsigned long B)
{
	return B ^ Rotl(B, 13) ^ Rotl(B, 23);
	// return B^(B<<13|B>>>19)^(B<<23|B>>>9);
}

static void SMS4Crypt(unsigned char *Input, unsigned char *Output, unsigned long *rk)
{
	int r,i;
	unsigned long mid;
	unsigned long x[4] = {0};
	unsigned char tmp[4] = {0};
	for (i = 0; i < 4; i++)
	{
		tmp[0] = Input[0 + 4 * i];
		tmp[1] = Input[1 + 4 * i];
		tmp[2] = Input[2 + 4 * i];
		tmp[3] = Input[3 + 4 * i];
		x[i] = ((unsigned long)tmp[0] << 24) | ((unsigned long)tmp[1] << 16) | ((unsigned long)tmp[2] << 8) | tmp[3];
		// x[i]=(Input[0+4*i]<<24|Input[1+4*i]<<16|Input[2+4*i]<<8|Input[3+4*i]);
	}
	for (r = 0; r < 32; r += 4)
	{
		mid = x[1] ^ x[2] ^ x[3] ^ rk[r + 0];
		mid = ByteSub(mid);
		x[0] = x[0] ^ L1(mid); // x4

		mid = x[2] ^ x[3] ^ x[0] ^ rk[r + 1];
		mid = ByteSub(mid);
		x[1] = x[1] ^ L1(mid); // x5

		mid = x[3] ^ x[0] ^ x[1] ^ rk[r + 2];
		mid = ByteSub(mid);
		x[2] = x[2] ^ L1(mid); // x6

		mid = x[0] ^ x[1] ^ x[2] ^ rk[r + 3];
		mid = ByteSub(mid);
		x[3] = x[3] ^ L1(mid); // x7
	}

	// Reverse
	for (int j = 0; j < 16; j += 4)
	{
		Output[j] = (x[3 - j / 4] >> 24) & 0xFF;
		Output[j + 1] = (x[3 - j / 4] >> 16) & 0xFF;
		Output[j + 2] = (x[3 - j / 4] >> 8) & 0xFF;
		Output[j + 3] = x[3 - j / 4] & 0xFF;
	}
}

static void SMS4KeyExt(unsigned char *Key, unsigned long * rk, unsigned char CryptFlag)
{
	unsigned long r, mid;
	unsigned long x[4] = {0};
	unsigned char tmp[4] = {0};
	for (int i = 0; i < 4; i++)
	{
		tmp[0] = Key[0 + 4 * i];
		tmp[1] = Key[1 + 4 * i];
		tmp[2] = Key[2 + 4 * i];
		tmp[3] = Key[3 + 4 * i];
		x[i] = (unsigned long)tmp[0] << 24 | (unsigned long)tmp[1] << 16 | (unsigned long)tmp[2] << 8 | tmp[3];
		// x[i]=Key[0+4*i]<<24|Key[1+4*i]<<16|Key[2+4*i]<<8|Key[3+4*i];
	}
	x[0] ^= FK[0];
	x[1] ^= FK[1];
	x[2] ^= FK[2];
	x[3] ^= FK[3];
	for (r = 0; r < 32; r += 4)
	{
		mid = x[1] ^ x[2] ^ x[3] ^ CK[r + 0];
		mid = ByteSub(mid);
		rk[r + 0] = x[0] ^= L2(mid); // rk0=K4

		mid = x[2] ^ x[3] ^ x[0] ^ CK[r + 1];
		mid = ByteSub(mid);
		rk[r + 1] = x[1] ^= L2(mid); // rk1=K5

		mid = x[3] ^ x[0] ^ x[1] ^ CK[r + 2];
		mid = ByteSub(mid);
		rk[r + 2] = x[2] ^= L2(mid); // rk2=K6

		mid = x[0] ^ x[1] ^ x[2] ^ CK[r + 3];
		mid = ByteSub(mid);
		rk[r + 3] = x[3] ^= L2(mid); // rk3=K7
	}

	// ?a?üê±???ü??ê1ó??3Dò￡ork31,rk30,...,rk0
	if (CryptFlag == SM_DECRYPT)
	{
		for (r = 0; r < 16; r++)
		{
			mid = rk[r];
			rk[r] = rk[31 - r];
			rk[31 - r] = mid;
		}
	}
}

static void xors(unsigned char *iv, unsigned char *Input, int InputLen )
{
	int i;
	for (i = 0; i < InputLen; i++)
	{
		Input[i] = Input[i] ^ iv[i];
	}
}


// inLen 必须是16的倍数 
int sms4_ecb(unsigned char * Input, int inLen, unsigned char *key, unsigned char *Output, int CryptFlag)
{
	int point = 0;
	unsigned long round_key[SM_ROUND] = {0}; 
	unsigned char input[SM_BLOCK] = {0};
	unsigned char output[SM_BLOCK] = {0};
	SMS4KeyExt(key, round_key, CryptFlag);

	while (inLen >= SM_BLOCK)
	{
		memcpy(input, Input + point, SM_BLOCK);
		SMS4Crypt(input, output, round_key);
		memcpy(Output + point, output, SM_BLOCK);
		inLen -= SM_BLOCK;
		point += SM_BLOCK;
	}

	return 0;
}

// inLen 必须是16的倍数 
int sms4_cbc(unsigned char *Input, int inLen, unsigned char *key, unsigned char *iv, unsigned char *Output, int CryptFlag)
{
	int point = 0;
	unsigned long round_key[SM_ROUND] = {0};
	unsigned char input[SM_BLOCK] = {0};
	unsigned char output[SM_BLOCK] = {0};
	unsigned char tmpIV[SM_BLOCK] = {0};
	SMS4KeyExt(key, round_key, CryptFlag);


	if (CryptFlag == SM_ENCRYPT)
	{
		memcpy(tmpIV, iv, SM_BLOCK);

		while (inLen >= SM_BLOCK)
		{
			memcpy(input,Input + point,SM_BLOCK);
			xors(tmpIV, input, SM_BLOCK);
			SMS4Crypt(input, output, round_key);
			memcpy(tmpIV,output,SM_BLOCK);
			memcpy(Output + point,output,SM_BLOCK);
			inLen -= SM_BLOCK;
			point += SM_BLOCK;
		}
	}
	else
	{
		memcpy(tmpIV, iv, SM_BLOCK);
		while (inLen >= SM_BLOCK)
		{
			memcpy(Input + point, input, SM_BLOCK);
			SMS4Crypt(input, output, round_key);
			xors(tmpIV, output, SM_BLOCK);
			memcpy(tmpIV, input, SM_BLOCK);
			memcpy(Output + point, output, SM_BLOCK);
			inLen -= SM_BLOCK;
			point += SM_BLOCK;
		}
	}

	return 0;
}

           