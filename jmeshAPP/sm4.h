#ifndef SM4_H
#define SM4_H

#define    SM_ENCRYPT  1
#define    SM_DECRYPT  0
#define    SM_ROUND    32
#define    SM_BLOCK    16

// inLen 必须是16的倍数 
int sms4_ecb(unsigned char * Input, int inLen, unsigned char *key, unsigned char *Output, int CryptFlag);

// inLen 必须是16的倍数 
int sms4_cbc(unsigned char *Input, int inLen, unsigned char *key, unsigned char *iv, unsigned char *Output, int CryptFlag);
#endif
