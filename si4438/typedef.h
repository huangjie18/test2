
#ifndef	_TYPEDEF_H_
#define	_TYPEDEF_H_

//#pragma	NOAREGS					//forbidden work registers'bank changed

#include <stdio.h>

/*******************************************************************************
			Macro Definition
*******************************************************************************/
typedef	unsigned char         u8;
typedef	unsigned short        u16;
typedef	unsigned long         u32;
typedef	unsigned long long    u64;

typedef	signed char           s8;
typedef	signed short          s16;
typedef	signed long           s32;
typedef	signed long  long     s64;

typedef	unsigned char         U8;
typedef	unsigned short        U16;
typedef	unsigned long         U32;
typedef	unsigned long long    U64;

typedef	signed char           S8;
typedef	signed short          S16;
typedef	signed long           S32;
typedef	signed long long      S64;






typedef signed char           int8;
typedef signed char           INT8;
typedef unsigned char         uint8;
typedef unsigned char         UINT8;
typedef unsigned short 	      UINT16;
typedef unsigned short 	      uint16;
typedef signed long           int32;
typedef signed long           INT32;
typedef unsigned long         uint32;
typedef unsigned long         UINT32;
typedef unsigned char         UCHAR;
typedef	unsigned char         uchar;
typedef unsigned short 	      USHORT;
typedef	unsigned short	      ushort;



// Î»²Ù×÷ºê

#define     clr_bit(x,y)    x &= ~(1 << y)
#define     set_bit(x,y)    x |=  (1 << y)
#define     xor_bit(x,y)    x ^=  (1 << y)
#define     test_bit(x,y)     (x & (1 << y))

#define     bit7                7
#define     bit6                6
#define     bit5                5
#define     bit4                4
#define     bit3                3
#define     bit2                2
#define     bit1                1
#define     bit0                0


#define	__no_init		__attribute__((section("NO_INIT"),zero_init))

/*******************************************************************************
		End File
*******************************************************************************/
#endif
