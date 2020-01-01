/*
		datatype.h
*/

#ifndef   _DATATYPE_H
#define  _DATATYPE_H

//typedef unsigned char bool;
typedef unsigned char		BOOL;
//typedef unsigned char	 	bool;
typedef signed char 		int8;
typedef signed char 	    INT8;
typedef unsigned char		uint8;
typedef unsigned char		UINT8;
typedef  unsigned char		UCHAR ;
typedef signed short		int16;
typedef signed short		INT16;
typedef unsigned short	    uint16;
typedef unsigned short	    UINT16;
typedef signed long 		int32;
typedef signed long 		INT32;
typedef unsigned long		uint32;
typedef unsigned long		UINT32;
//typedef signed long long	int64;
//typedef signed long long	INT64;
//typedef unsigned long long	uint64;
//typedef unsigned long long	UINT64;

typedef	unsigned long	ULONG;
typedef	unsigned long	ulong;
typedef	unsigned int	UINT;
typedef	unsigned int	uint;
typedef	unsigned short	USHORT;
typedef	unsigned short	ushort;
typedef	unsigned char	uchar;
typedef	unsigned char	int8u;

typedef struct
{
	UINT8 b0:1;
	UINT8 b1:1;
	UINT8 b2:1;
	UINT8 b3:1;
	UINT8 b4:1;
	UINT8 b5:1;
	UINT8 b6:1;
	UINT8 b7:1;
} Byte_Bits;

typedef struct
{
	UINT8 b0:1;
	UINT8 b1:1;
	UINT8 b2:1;
	UINT8 b3:1;
	UINT8 b4:1;
	UINT8 b5:1;
	UINT8 b6:1;
	UINT8 b7:1;
	UINT8 b8:1;
	UINT8 b9:1;
	UINT8 b10:1;
	UINT8 b11:1;
	UINT8 b12:1;
	UINT8 b13:1;
	UINT8 b14:1;
	UINT8 b15:1;
} Word_Bits;

typedef  struct
{
	UINT8 b0:1;
	UINT8 b1:1;
	UINT8 b2:1;
	UINT8 b3:1;
	UINT8 b4:1;
	UINT8 b5:1;
	UINT8 b6:1;
	UINT8 b7:1;
	UINT8 b8:1;
	UINT8 b9:1;
	UINT8 b10:1;
	UINT8 b11:1;
	UINT8 b12:1;
	UINT8 b13:1;
	UINT8 b14:1;
	UINT8 b15:1;
	UINT8 b16:1;
	UINT8 b17:1;
	UINT8 b18:1;
	UINT8 b19:1;
	UINT8 b20:1;
	UINT8 b21:1;
	UINT8 b22:1;
	UINT8 b23:1;
	UINT8 b24:1;
	UINT8 b25:1;
	UINT8 b26:1;
	UINT8 b27:1;
	UINT8 b28:1;
	UINT8 b29:1;
	UINT8 b30:1;
	UINT8 b31:1;
} Long_Bits;


typedef union
{
	UINT8	keyByte;
	Byte_Bits keyBit;
} ByteBit;


typedef union
{
	UINT8	KeyByte[4];
	UINT32	KeyDWord;
} DWordByte;

typedef union
{
	UINT16	keyWord;
	Word_Bits keyBit;
} WordBit;


typedef union
{
	UINT32	keyLong;
	UINT16	keyWord[2];
} LongWord;

typedef union
{
	UINT32	keyLong;
	UINT8	keyByte[4];
} LongByte;

typedef union
{
	UINT32	keyLong;
	Word_Bits keyBit;
} LongBit;




#define  true  1
#define  false	 0

#define  OK   1
#define  EEROR	 0
#define fail 0
#define  Fail	 0
#define  Success  1

#define  HASH_ERR	0xffff

#define  APS_LAYER			2	
#define  NWK_LAYER			1	
#define  MAX_REPNUM_NUM 	8	





#endif/*datatype.h*/




