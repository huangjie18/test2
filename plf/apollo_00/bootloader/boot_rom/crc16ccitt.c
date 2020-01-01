#include <stdint.h>
/* refer to github:crcspeed */
#define CRC16_TABLE_SIZE 4096
uint8_t const crc16ccitt_table[CRC16_TABLE_SIZE] = {
0x00,0x00,0x21,0x10,0x42,0x20,0x63,0x30,0x84,0x40,0xA5,0x50,0xC6,0x60,0xE7,0x70,
0x08,0x81,0x29,0x91,0x4A,0xA1,0x6B,0xB1,0x8C,0xC1,0xAD,0xD1,0xCE,0xE1,0xEF,0xF1,
0x31,0x12,0x10,0x02,0x73,0x32,0x52,0x22,0xB5,0x52,0x94,0x42,0xF7,0x72,0xD6,0x62,
0x39,0x93,0x18,0x83,0x7B,0xB3,0x5A,0xA3,0xBD,0xD3,0x9C,0xC3,0xFF,0xF3,0xDE,0xE3,
0x62,0x24,0x43,0x34,0x20,0x04,0x01,0x14,0xE6,0x64,0xC7,0x74,0xA4,0x44,0x85,0x54,
0x6A,0xA5,0x4B,0xB5,0x28,0x85,0x09,0x95,0xEE,0xE5,0xCF,0xF5,0xAC,0xC5,0x8D,0xD5,
0x53,0x36,0x72,0x26,0x11,0x16,0x30,0x06,0xD7,0x76,0xF6,0x66,0x95,0x56,0xB4,0x46,
0x5B,0xB7,0x7A,0xA7,0x19,0x97,0x38,0x87,0xDF,0xF7,0xFE,0xE7,0x9D,0xD7,0xBC,0xC7,
0xC4,0x48,0xE5,0x58,0x86,0x68,0xA7,0x78,0x40,0x08,0x61,0x18,0x02,0x28,0x23,0x38,
0xCC,0xC9,0xED,0xD9,0x8E,0xE9,0xAF,0xF9,0x48,0x89,0x69,0x99,0x0A,0xA9,0x2B,0xB9,
0xF5,0x5A,0xD4,0x4A,0xB7,0x7A,0x96,0x6A,0x71,0x1A,0x50,0x0A,0x33,0x3A,0x12,0x2A,
0xFD,0xDB,0xDC,0xCB,0xBF,0xFB,0x9E,0xEB,0x79,0x9B,0x58,0x8B,0x3B,0xBB,0x1A,0xAB,
0xA6,0x6C,0x87,0x7C,0xE4,0x4C,0xC5,0x5C,0x22,0x2C,0x03,0x3C,0x60,0x0C,0x41,0x1C,
0xAE,0xED,0x8F,0xFD,0xEC,0xCD,0xCD,0xDD,0x2A,0xAD,0x0B,0xBD,0x68,0x8D,0x49,0x9D,
0x97,0x7E,0xB6,0x6E,0xD5,0x5E,0xF4,0x4E,0x13,0x3E,0x32,0x2E,0x51,0x1E,0x70,0x0E,
0x9F,0xFF,0xBE,0xEF,0xDD,0xDF,0xFC,0xCF,0x1B,0xBF,0x3A,0xAF,0x59,0x9F,0x78,0x8F,
0x88,0x91,0xA9,0x81,0xCA,0xB1,0xEB,0xA1,0x0C,0xD1,0x2D,0xC1,0x4E,0xF1,0x6F,0xE1,
0x80,0x10,0xA1,0x00,0xC2,0x30,0xE3,0x20,0x04,0x50,0x25,0x40,0x46,0x70,0x67,0x60,
0xB9,0x83,0x98,0x93,0xFB,0xA3,0xDA,0xB3,0x3D,0xC3,0x1C,0xD3,0x7F,0xE3,0x5E,0xF3,
0xB1,0x02,0x90,0x12,0xF3,0x22,0xD2,0x32,0x35,0x42,0x14,0x52,0x77,0x62,0x56,0x72,
0xEA,0xB5,0xCB,0xA5,0xA8,0x95,0x89,0x85,0x6E,0xF5,0x4F,0xE5,0x2C,0xD5,0x0D,0xC5,
0xE2,0x34,0xC3,0x24,0xA0,0x14,0x81,0x04,0x66,0x74,0x47,0x64,0x24,0x54,0x05,0x44,
0xDB,0xA7,0xFA,0xB7,0x99,0x87,0xB8,0x97,0x5F,0xE7,0x7E,0xF7,0x1D,0xC7,0x3C,0xD7,
0xD3,0x26,0xF2,0x36,0x91,0x06,0xB0,0x16,0x57,0x66,0x76,0x76,0x15,0x46,0x34,0x56,
0x4C,0xD9,0x6D,0xC9,0x0E,0xF9,0x2F,0xE9,0xC8,0x99,0xE9,0x89,0x8A,0xB9,0xAB,0xA9,
0x44,0x58,0x65,0x48,0x06,0x78,0x27,0x68,0xC0,0x18,0xE1,0x08,0x82,0x38,0xA3,0x28,
0x7D,0xCB,0x5C,0xDB,0x3F,0xEB,0x1E,0xFB,0xF9,0x8B,0xD8,0x9B,0xBB,0xAB,0x9A,0xBB,
0x75,0x4A,0x54,0x5A,0x37,0x6A,0x16,0x7A,0xF1,0x0A,0xD0,0x1A,0xB3,0x2A,0x92,0x3A,
0x2E,0xFD,0x0F,0xED,0x6C,0xDD,0x4D,0xCD,0xAA,0xBD,0x8B,0xAD,0xE8,0x9D,0xC9,0x8D,
0x26,0x7C,0x07,0x6C,0x64,0x5C,0x45,0x4C,0xA2,0x3C,0x83,0x2C,0xE0,0x1C,0xC1,0x0C,
0x1F,0xEF,0x3E,0xFF,0x5D,0xCF,0x7C,0xDF,0x9B,0xAF,0xBA,0xBF,0xD9,0x8F,0xF8,0x9F,
0x17,0x6E,0x36,0x7E,0x55,0x4E,0x74,0x5E,0x93,0x2E,0xB2,0x3E,0xD1,0x0E,0xF0,0x1E,
0x00,0x00,0x31,0x33,0x62,0x66,0x53,0x55,0xC4,0xCC,0xF5,0xFF,0xA6,0xAA,0x97,0x99,
0xA9,0x89,0x98,0xBA,0xCB,0xEF,0xFA,0xDC,0x6D,0x45,0x5C,0x76,0x0F,0x23,0x3E,0x10,
0x73,0x03,0x42,0x30,0x11,0x65,0x20,0x56,0xB7,0xCF,0x86,0xFC,0xD5,0xA9,0xE4,0x9A,
0xDA,0x8A,0xEB,0xB9,0xB8,0xEC,0x89,0xDF,0x1E,0x46,0x2F,0x75,0x7C,0x20,0x4D,0x13,
0xE6,0x06,0xD7,0x35,0x84,0x60,0xB5,0x53,0x22,0xCA,0x13,0xF9,0x40,0xAC,0x71,0x9F,
0x4F,0x8F,0x7E,0xBC,0x2D,0xE9,0x1C,0xDA,0x8B,0x43,0xBA,0x70,0xE9,0x25,0xD8,0x16,
0x95,0x05,0xA4,0x36,0xF7,0x63,0xC6,0x50,0x51,0xC9,0x60,0xFA,0x33,0xAF,0x02,0x9C,
0x3C,0x8C,0x0D,0xBF,0x5E,0xEA,0x6F,0xD9,0xF8,0x40,0xC9,0x73,0x9A,0x26,0xAB,0x15,
0xCC,0x0D,0xFD,0x3E,0xAE,0x6B,0x9F,0x58,0x08,0xC1,0x39,0xF2,0x6A,0xA7,0x5B,0x94,
0x65,0x84,0x54,0xB7,0x07,0xE2,0x36,0xD1,0xA1,0x48,0x90,0x7B,0xC3,0x2E,0xF2,0x1D,
0xBF,0x0E,0x8E,0x3D,0xDD,0x68,0xEC,0x5B,0x7B,0xC2,0x4A,0xF1,0x19,0xA4,0x28,0x97,
0x16,0x87,0x27,0xB4,0x74,0xE1,0x45,0xD2,0xD2,0x4B,0xE3,0x78,0xB0,0x2D,0x81,0x1E,
0x2A,0x0B,0x1B,0x38,0x48,0x6D,0x79,0x5E,0xEE,0xC7,0xDF,0xF4,0x8C,0xA1,0xBD,0x92,
0x83,0x82,0xB2,0xB1,0xE1,0xE4,0xD0,0xD7,0x47,0x4E,0x76,0x7D,0x25,0x28,0x14,0x1B,
0x59,0x08,0x68,0x3B,0x3B,0x6E,0x0A,0x5D,0x9D,0xC4,0xAC,0xF7,0xFF,0xA2,0xCE,0x91,
0xF0,0x81,0xC1,0xB2,0x92,0xE7,0xA3,0xD4,0x34,0x4D,0x05,0x7E,0x56,0x2B,0x67,0x18,
0x98,0x1B,0xA9,0x28,0xFA,0x7D,0xCB,0x4E,0x5C,0xD7,0x6D,0xE4,0x3E,0xB1,0x0F,0x82,
0x31,0x92,0x00,0xA1,0x53,0xF4,0x62,0xC7,0xF5,0x5E,0xC4,0x6D,0x97,0x38,0xA6,0x0B,
0xEB,0x18,0xDA,0x2B,0x89,0x7E,0xB8,0x4D,0x2F,0xD4,0x1E,0xE7,0x4D,0xB2,0x7C,0x81,
0x42,0x91,0x73,0xA2,0x20,0xF7,0x11,0xC4,0x86,0x5D,0xB7,0x6E,0xE4,0x3B,0xD5,0x08,
0x7E,0x1D,0x4F,0x2E,0x1C,0x7B,0x2D,0x48,0xBA,0xD1,0x8B,0xE2,0xD8,0xB7,0xE9,0x84,
0xD7,0x94,0xE6,0xA7,0xB5,0xF2,0x84,0xC1,0x13,0x58,0x22,0x6B,0x71,0x3E,0x40,0x0D,
0x0D,0x1E,0x3C,0x2D,0x6F,0x78,0x5E,0x4B,0xC9,0xD2,0xF8,0xE1,0xAB,0xB4,0x9A,0x87,
0xA4,0x97,0x95,0xA4,0xC6,0xF1,0xF7,0xC2,0x60,0x5B,0x51,0x68,0x02,0x3D,0x33,0x0E,
0x54,0x16,0x65,0x25,0x36,0x70,0x07,0x43,0x90,0xDA,0xA1,0xE9,0xF2,0xBC,0xC3,0x8F,
0xFD,0x9F,0xCC,0xAC,0x9F,0xF9,0xAE,0xCA,0x39,0x53,0x08,0x60,0x5B,0x35,0x6A,0x06,
0x27,0x15,0x16,0x26,0x45,0x73,0x74,0x40,0xE3,0xD9,0xD2,0xEA,0x81,0xBF,0xB0,0x8C,
0x8E,0x9C,0xBF,0xAF,0xEC,0xFA,0xDD,0xC9,0x4A,0x50,0x7B,0x63,0x28,0x36,0x19,0x05,
0xB2,0x10,0x83,0x23,0xD0,0x76,0xE1,0x45,0x76,0xDC,0x47,0xEF,0x14,0xBA,0x25,0x89,
0x1B,0x99,0x2A,0xAA,0x79,0xFF,0x48,0xCC,0xDF,0x55,0xEE,0x66,0xBD,0x33,0x8C,0x00,
0xC1,0x13,0xF0,0x20,0xA3,0x75,0x92,0x46,0x05,0xDF,0x34,0xEC,0x67,0xB9,0x56,0x8A,
0x68,0x9A,0x59,0xA9,0x0A,0xFC,0x3B,0xCF,0xAC,0x56,0x9D,0x65,0xCE,0x30,0xFF,0x03,
0x00,0x00,0x30,0x37,0x60,0x6E,0x50,0x59,0xC0,0xDC,0xF0,0xEB,0xA0,0xB2,0x90,0x85,
0xA1,0xA9,0x91,0x9E,0xC1,0xC7,0xF1,0xF0,0x61,0x75,0x51,0x42,0x01,0x1B,0x31,0x2C,
0x63,0x43,0x53,0x74,0x03,0x2D,0x33,0x1A,0xA3,0x9F,0x93,0xA8,0xC3,0xF1,0xF3,0xC6,
0xC2,0xEA,0xF2,0xDD,0xA2,0x84,0x92,0xB3,0x02,0x36,0x32,0x01,0x62,0x58,0x52,0x6F,
0xC6,0x86,0xF6,0xB1,0xA6,0xE8,0x96,0xDF,0x06,0x5A,0x36,0x6D,0x66,0x34,0x56,0x03,
0x67,0x2F,0x57,0x18,0x07,0x41,0x37,0x76,0xA7,0xF3,0x97,0xC4,0xC7,0x9D,0xF7,0xAA,
0xA5,0xC5,0x95,0xF2,0xC5,0xAB,0xF5,0x9C,0x65,0x19,0x55,0x2E,0x05,0x77,0x35,0x40,
0x04,0x6C,0x34,0x5B,0x64,0x02,0x54,0x35,0xC4,0xB0,0xF4,0x87,0xA4,0xDE,0x94,0xE9,
0xAD,0x1D,0x9D,0x2A,0xCD,0x73,0xFD,0x44,0x6D,0xC1,0x5D,0xF6,0x0D,0xAF,0x3D,0x98,
0x0C,0xB4,0x3C,0x83,0x6C,0xDA,0x5C,0xED,0xCC,0x68,0xFC,0x5F,0xAC,0x06,0x9C,0x31,
0xCE,0x5E,0xFE,0x69,0xAE,0x30,0x9E,0x07,0x0E,0x82,0x3E,0xB5,0x6E,0xEC,0x5E,0xDB,
0x6F,0xF7,0x5F,0xC0,0x0F,0x99,0x3F,0xAE,0xAF,0x2B,0x9F,0x1C,0xCF,0x45,0xFF,0x72,
0x6B,0x9B,0x5B,0xAC,0x0B,0xF5,0x3B,0xC2,0xAB,0x47,0x9B,0x70,0xCB,0x29,0xFB,0x1E,
0xCA,0x32,0xFA,0x05,0xAA,0x5C,0x9A,0x6B,0x0A,0xEE,0x3A,0xD9,0x6A,0x80,0x5A,0xB7,
0x08,0xD8,0x38,0xEF,0x68,0xB6,0x58,0x81,0xC8,0x04,0xF8,0x33,0xA8,0x6A,0x98,0x5D,
0xA9,0x71,0x99,0x46,0xC9,0x1F,0xF9,0x28,0x69,0xAD,0x59,0x9A,0x09,0xC3,0x39,0xF4,
0x5A,0x3B,0x6A,0x0C,0x3A,0x55,0x0A,0x62,0x9A,0xE7,0xAA,0xD0,0xFA,0x89,0xCA,0xBE,
0xFB,0x92,0xCB,0xA5,0x9B,0xFC,0xAB,0xCB,0x3B,0x4E,0x0B,0x79,0x5B,0x20,0x6B,0x17,
0x39,0x78,0x09,0x4F,0x59,0x16,0x69,0x21,0xF9,0xA4,0xC9,0x93,0x99,0xCA,0xA9,0xFD,
0x98,0xD1,0xA8,0xE6,0xF8,0xBF,0xC8,0x88,0x58,0x0D,0x68,0x3A,0x38,0x63,0x08,0x54,
0x9C,0xBD,0xAC,0x8A,0xFC,0xD3,0xCC,0xE4,0x5C,0x61,0x6C,0x56,0x3C,0x0F,0x0C,0x38,
0x3D,0x14,0x0D,0x23,0x5D,0x7A,0x6D,0x4D,0xFD,0xC8,0xCD,0xFF,0x9D,0xA6,0xAD,0x91,
0xFF,0xFE,0xCF,0xC9,0x9F,0x90,0xAF,0xA7,0x3F,0x22,0x0F,0x15,0x5F,0x4C,0x6F,0x7B,
0x5E,0x57,0x6E,0x60,0x3E,0x39,0x0E,0x0E,0x9E,0x8B,0xAE,0xBC,0xFE,0xE5,0xCE,0xD2,
0xF7,0x26,0xC7,0x11,0x97,0x48,0xA7,0x7F,0x37,0xFA,0x07,0xCD,0x57,0x94,0x67,0xA3,
0x56,0x8F,0x66,0xB8,0x36,0xE1,0x06,0xD6,0x96,0x53,0xA6,0x64,0xF6,0x3D,0xC6,0x0A,
0x94,0x65,0xA4,0x52,0xF4,0x0B,0xC4,0x3C,0x54,0xB9,0x64,0x8E,0x34,0xD7,0x04,0xE0,
0x35,0xCC,0x05,0xFB,0x55,0xA2,0x65,0x95,0xF5,0x10,0xC5,0x27,0x95,0x7E,0xA5,0x49,
0x31,0xA0,0x01,0x97,0x51,0xCE,0x61,0xF9,0xF1,0x7C,0xC1,0x4B,0x91,0x12,0xA1,0x25,
0x90,0x09,0xA0,0x3E,0xF0,0x67,0xC0,0x50,0x50,0xD5,0x60,0xE2,0x30,0xBB,0x00,0x8C,
0x52,0xE3,0x62,0xD4,0x32,0x8D,0x02,0xBA,0x92,0x3F,0xA2,0x08,0xF2,0x51,0xC2,0x66,
0xF3,0x4A,0xC3,0x7D,0x93,0x24,0xA3,0x13,0x33,0x96,0x03,0xA1,0x53,0xF8,0x63,0xCF,
0x00,0x00,0xB4,0x76,0x68,0xED,0xDC,0x9B,0xF1,0xCA,0x45,0xBC,0x99,0x27,0x2D,0x51,
0xC3,0x85,0x77,0xF3,0xAB,0x68,0x1F,0x1E,0x32,0x4F,0x86,0x39,0x5A,0xA2,0xEE,0xD4,
0xA7,0x1B,0x13,0x6D,0xCF,0xF6,0x7B,0x80,0x56,0xD1,0xE2,0xA7,0x3E,0x3C,0x8A,0x4A,
0x64,0x9E,0xD0,0xE8,0x0C,0x73,0xB8,0x05,0x95,0x54,0x21,0x22,0xFD,0xB9,0x49,0xCF,
0x4E,0x37,0xFA,0x41,0x26,0xDA,0x92,0xAC,0xBF,0xFD,0x0B,0x8B,0xD7,0x10,0x63,0x66,
0x8D,0xB2,0x39,0xC4,0xE5,0x5F,0x51,0x29,0x7C,0x78,0xC8,0x0E,0x14,0x95,0xA0,0xE3,
0xE9,0x2C,0x5D,0x5A,0x81,0xC1,0x35,0xB7,0x18,0xE6,0xAC,0x90,0x70,0x0B,0xC4,0x7D,
0x2A,0xA9,0x9E,0xDF,0x42,0x44,0xF6,0x32,0xDB,0x63,0x6F,0x15,0xB3,0x8E,0x07,0xF8,
0x9C,0x6E,0x28,0x18,0xF4,0x83,0x40,0xF5,0x6D,0xA4,0xD9,0xD2,0x05,0x49,0xB1,0x3F,
0x5F,0xEB,0xEB,0x9D,0x37,0x06,0x83,0x70,0xAE,0x21,0x1A,0x57,0xC6,0xCC,0x72,0xBA,
0x3B,0x75,0x8F,0x03,0x53,0x98,0xE7,0xEE,0xCA,0xBF,0x7E,0xC9,0xA2,0x52,0x16,0x24,
0xF8,0xF0,0x4C,0x86,0x90,0x1D,0x24,0x6B,0x09,0x3A,0xBD,0x4C,0x61,0xD7,0xD5,0xA1,
0xD2,0x59,0x66,0x2F,0xBA,0xB4,0x0E,0xC2,0x23,0x93,0x97,0xE5,0x4B,0x7E,0xFF,0x08,
0x11,0xDC,0xA5,0xAA,0x79,0x31,0xCD,0x47,0xE0,0x16,0x54,0x60,0x88,0xFB,0x3C,0x8D,
0x75,0x42,0xC1,0x34,0x1D,0xAF,0xA9,0xD9,0x84,0x88,0x30,0xFE,0xEC,0x65,0x58,0x13,
0xB6,0xC7,0x02,0xB1,0xDE,0x2A,0x6A,0x5C,0x47,0x0D,0xF3,0x7B,0x2F,0xE0,0x9B,0x96,
0x38,0xDD,0x8C,0xAB,0x50,0x30,0xE4,0x46,0xC9,0x17,0x7D,0x61,0xA1,0xFA,0x15,0x8C,
0xFB,0x58,0x4F,0x2E,0x93,0xB5,0x27,0xC3,0x0A,0x92,0xBE,0xE4,0x62,0x7F,0xD6,0x09,
0x9F,0xC6,0x2B,0xB0,0xF7,0x2B,0x43,0x5D,0x6E,0x0C,0xDA,0x7A,0x06,0xE1,0xB2,0x97,
0x5C,0x43,0xE8,0x35,0x34,0xAE,0x80,0xD8,0xAD,0x89,0x19,0xFF,0xC5,0x64,0x71,0x12,
0x76,0xEA,0xC2,0x9C,0x1E,0x07,0xAA,0x71,0x87,0x20,0x33,0x56,0xEF,0xCD,0x5B,0xBB,
0xB5,0x6F,0x01,0x19,0xDD,0x82,0x69,0xF4,0x44,0xA5,0xF0,0xD3,0x2C,0x48,0x98,0x3E,
0xD1,0xF1,0x65,0x87,0xB9,0x1C,0x0D,0x6A,0x20,0x3B,0x94,0x4D,0x48,0xD6,0xFC,0xA0,
0x12,0x74,0xA6,0x02,0x7A,0x99,0xCE,0xEF,0xE3,0xBE,0x57,0xC8,0x8B,0x53,0x3F,0x25,
0xA4,0xB3,0x10,0xC5,0xCC,0x5E,0x78,0x28,0x55,0x79,0xE1,0x0F,0x3D,0x94,0x89,0xE2,
0x67,0x36,0xD3,0x40,0x0F,0xDB,0xBB,0xAD,0x96,0xFC,0x22,0x8A,0xFE,0x11,0x4A,0x67,
0x03,0xA8,0xB7,0xDE,0x6B,0x45,0xDF,0x33,0xF2,0x62,0x46,0x14,0x9A,0x8F,0x2E,0xF9,
0xC0,0x2D,0x74,0x5B,0xA8,0xC0,0x1C,0xB6,0x31,0xE7,0x85,0x91,0x59,0x0A,0xED,0x7C,
0xEA,0x84,0x5E,0xF2,0x82,0x69,0x36,0x1F,0x1B,0x4E,0xAF,0x38,0x73,0xA3,0xC7,0xD5,
0x29,0x01,0x9D,0x77,0x41,0xEC,0xF5,0x9A,0xD8,0xCB,0x6C,0xBD,0xB0,0x26,0x04,0x50,
0x4D,0x9F,0xF9,0xE9,0x25,0x72,0x91,0x04,0xBC,0x55,0x08,0x23,0xD4,0xB8,0x60,0xCE,
0x8E,0x1A,0x3A,0x6C,0xE6,0xF7,0x52,0x81,0x7F,0xD0,0xCB,0xA6,0x17,0x3D,0xA3,0x4B,
0x00,0x00,0x51,0xAA,0x83,0x44,0xD2,0xEE,0x06,0x89,0x57,0x23,0x85,0xCD,0xD4,0x67,
0x2D,0x02,0x7C,0xA8,0xAE,0x46,0xFF,0xEC,0x2B,0x8B,0x7A,0x21,0xA8,0xCF,0xF9,0x65,
0x5A,0x04,0x0B,0xAE,0xD9,0x40,0x88,0xEA,0x5C,0x8D,0x0D,0x27,0xDF,0xC9,0x8E,0x63,
0x77,0x06,0x26,0xAC,0xF4,0x42,0xA5,0xE8,0x71,0x8F,0x20,0x25,0xF2,0xCB,0xA3,0x61,
0xB4,0x08,0xE5,0xA2,0x37,0x4C,0x66,0xE6,0xB2,0x81,0xE3,0x2B,0x31,0xC5,0x60,0x6F,
0x99,0x0A,0xC8,0xA0,0x1A,0x4E,0x4B,0xE4,0x9F,0x83,0xCE,0x29,0x1C,0xC7,0x4D,0x6D,
0xEE,0x0C,0xBF,0xA6,0x6D,0x48,0x3C,0xE2,0xE8,0x85,0xB9,0x2F,0x6B,0xC1,0x3A,0x6B,
0xC3,0x0E,0x92,0xA4,0x40,0x4A,0x11,0xE0,0xC5,0x87,0x94,0x2D,0x46,0xC3,0x17,0x69,
0x68,0x11,0x39,0xBB,0xEB,0x55,0xBA,0xFF,0x6E,0x98,0x3F,0x32,0xED,0xDC,0xBC,0x76,
0x45,0x13,0x14,0xB9,0xC6,0x57,0x97,0xFD,0x43,0x9A,0x12,0x30,0xC0,0xDE,0x91,0x74,
0x32,0x15,0x63,0xBF,0xB1,0x51,0xE0,0xFB,0x34,0x9C,0x65,0x36,0xB7,0xD8,0xE6,0x72,
0x1F,0x17,0x4E,0xBD,0x9C,0x53,0xCD,0xF9,0x19,0x9E,0x48,0x34,0x9A,0xDA,0xCB,0x70,
0xDC,0x19,0x8D,0xB3,0x5F,0x5D,0x0E,0xF7,0xDA,0x90,0x8B,0x3A,0x59,0xD4,0x08,0x7E,
0xF1,0x1B,0xA0,0xB1,0x72,0x5F,0x23,0xF5,0xF7,0x92,0xA6,0x38,0x74,0xD6,0x25,0x7C,
0x86,0x1D,0xD7,0xB7,0x05,0x59,0x54,0xF3,0x80,0x94,0xD1,0x3E,0x03,0xD0,0x52,0x7A,
0xAB,0x1F,0xFA,0xB5,0x28,0x5B,0x79,0xF1,0xAD,0x96,0xFC,0x3C,0x2E,0xD2,0x7F,0x78,
0xD0,0x22,0x81,0x88,0x53,0x66,0x02,0xCC,0xD6,0xAB,0x87,0x01,0x55,0xEF,0x04,0x45,
0xFD,0x20,0xAC,0x8A,0x7E,0x64,0x2F,0xCE,0xFB,0xA9,0xAA,0x03,0x78,0xED,0x29,0x47,
0x8A,0x26,0xDB,0x8C,0x09,0x62,0x58,0xC8,0x8C,0xAF,0xDD,0x05,0x0F,0xEB,0x5E,0x41,
0xA7,0x24,0xF6,0x8E,0x24,0x60,0x75,0xCA,0xA1,0xAD,0xF0,0x07,0x22,0xE9,0x73,0x43,
0x64,0x2A,0x35,0x80,0xE7,0x6E,0xB6,0xC4,0x62,0xA3,0x33,0x09,0xE1,0xE7,0xB0,0x4D,
0x49,0x28,0x18,0x82,0xCA,0x6C,0x9B,0xC6,0x4F,0xA1,0x1E,0x0B,0xCC,0xE5,0x9D,0x4F,
0x3E,0x2E,0x6F,0x84,0xBD,0x6A,0xEC,0xC0,0x38,0xA7,0x69,0x0D,0xBB,0xE3,0xEA,0x49,
0x13,0x2C,0x42,0x86,0x90,0x68,0xC1,0xC2,0x15,0xA5,0x44,0x0F,0x96,0xE1,0xC7,0x4B,
0xB8,0x33,0xE9,0x99,0x3B,0x77,0x6A,0xDD,0xBE,0xBA,0xEF,0x10,0x3D,0xFE,0x6C,0x54,
0x95,0x31,0xC4,0x9B,0x16,0x75,0x47,0xDF,0x93,0xB8,0xC2,0x12,0x10,0xFC,0x41,0x56,
0xE2,0x37,0xB3,0x9D,0x61,0x73,0x30,0xD9,0xE4,0xBE,0xB5,0x14,0x67,0xFA,0x36,0x50,
0xCF,0x35,0x9E,0x9F,0x4C,0x71,0x1D,0xDB,0xC9,0xBC,0x98,0x16,0x4A,0xF8,0x1B,0x52,
0x0C,0x3B,0x5D,0x91,0x8F,0x7F,0xDE,0xD5,0x0A,0xB2,0x5B,0x18,0x89,0xF6,0xD8,0x5C,
0x21,0x39,0x70,0x93,0xA2,0x7D,0xF3,0xD7,0x27,0xB0,0x76,0x1A,0xA4,0xF4,0xF5,0x5E,
0x56,0x3F,0x07,0x95,0xD5,0x7B,0x84,0xD1,0x50,0xB6,0x01,0x1C,0xD3,0xF2,0x82,0x58,
0x7B,0x3D,0x2A,0x97,0xF8,0x79,0xA9,0xD3,0x7D,0xB4,0x2C,0x1E,0xFE,0xF0,0xAF,0x5A,
0x00,0x00,0xA0,0x45,0x40,0x8B,0xE0,0xCE,0xA1,0x06,0x01,0x43,0xE1,0x8D,0x41,0xC8,
0x42,0x0D,0xE2,0x48,0x02,0x86,0xA2,0xC3,0xE3,0x0B,0x43,0x4E,0xA3,0x80,0x03,0xC5,
0x84,0x1A,0x24,0x5F,0xC4,0x91,0x64,0xD4,0x25,0x1C,0x85,0x59,0x65,0x97,0xC5,0xD2,
0xC6,0x17,0x66,0x52,0x86,0x9C,0x26,0xD9,0x67,0x11,0xC7,0x54,0x27,0x9A,0x87,0xDF,
0x08,0x35,0xA8,0x70,0x48,0xBE,0xE8,0xFB,0xA9,0x33,0x09,0x76,0xE9,0xB8,0x49,0xFD,
0x4A,0x38,0xEA,0x7D,0x0A,0xB3,0xAA,0xF6,0xEB,0x3E,0x4B,0x7B,0xAB,0xB5,0x0B,0xF0,
0x8C,0x2F,0x2C,0x6A,0xCC,0xA4,0x6C,0xE1,0x2D,0x29,0x8D,0x6C,0x6D,0xA2,0xCD,0xE7,
0xCE,0x22,0x6E,0x67,0x8E,0xA9,0x2E,0xEC,0x6F,0x24,0xCF,0x61,0x2F,0xAF,0x8F,0xEA,
0x10,0x6A,0xB0,0x2F,0x50,0xE1,0xF0,0xA4,0xB1,0x6C,0x11,0x29,0xF1,0xE7,0x51,0xA2,
0x52,0x67,0xF2,0x22,0x12,0xEC,0xB2,0xA9,0xF3,0x61,0x53,0x24,0xB3,0xEA,0x13,0xAF,
0x94,0x70,0x34,0x35,0xD4,0xFB,0x74,0xBE,0x35,0x76,0x95,0x33,0x75,0xFD,0xD5,0xB8,
0xD6,0x7D,0x76,0x38,0x96,0xF6,0x36,0xB3,0x77,0x7B,0xD7,0x3E,0x37,0xF0,0x97,0xB5,
0x18,0x5F,0xB8,0x1A,0x58,0xD4,0xF8,0x91,0xB9,0x59,0x19,0x1C,0xF9,0xD2,0x59,0x97,
0x5A,0x52,0xFA,0x17,0x1A,0xD9,0xBA,0x9C,0xFB,0x54,0x5B,0x11,0xBB,0xDF,0x1B,0x9A,
0x9C,0x45,0x3C,0x00,0xDC,0xCE,0x7C,0x8B,0x3D,0x43,0x9D,0x06,0x7D,0xC8,0xDD,0x8D,
0xDE,0x48,0x7E,0x0D,0x9E,0xC3,0x3E,0x86,0x7F,0x4E,0xDF,0x0B,0x3F,0xC5,0x9F,0x80,
0x20,0xD4,0x80,0x91,0x60,0x5F,0xC0,0x1A,0x81,0xD2,0x21,0x97,0xC1,0x59,0x61,0x1C,
0x62,0xD9,0xC2,0x9C,0x22,0x52,0x82,0x17,0xC3,0xDF,0x63,0x9A,0x83,0x54,0x23,0x11,
0xA4,0xCE,0x04,0x8B,0xE4,0x45,0x44,0x00,0x05,0xC8,0xA5,0x8D,0x45,0x43,0xE5,0x06,
0xE6,0xC3,0x46,0x86,0xA6,0x48,0x06,0x0D,0x47,0xC5,0xE7,0x80,0x07,0x4E,0xA7,0x0B,
0x28,0xE1,0x88,0xA4,0x68,0x6A,0xC8,0x2F,0x89,0xE7,0x29,0xA2,0xC9,0x6C,0x69,0x29,
0x6A,0xEC,0xCA,0xA9,0x2A,0x67,0x8A,0x22,0xCB,0xEA,0x6B,0xAF,0x8B,0x61,0x2B,0x24,
0xAC,0xFB,0x0C,0xBE,0xEC,0x70,0x4C,0x35,0x0D,0xFD,0xAD,0xB8,0x4D,0x76,0xED,0x33,
0xEE,0xF6,0x4E,0xB3,0xAE,0x7D,0x0E,0x38,0x4F,0xF0,0xEF,0xB5,0x0F,0x7B,0xAF,0x3E,
0x30,0xBE,0x90,0xFB,0x70,0x35,0xD0,0x70,0x91,0xB8,0x31,0xFD,0xD1,0x33,0x71,0x76,
0x72,0xB3,0xD2,0xF6,0x32,0x38,0x92,0x7D,0xD3,0xB5,0x73,0xF0,0x93,0x3E,0x33,0x7B,
0xB4,0xA4,0x14,0xE1,0xF4,0x2F,0x54,0x6A,0x15,0xA2,0xB5,0xE7,0x55,0x29,0xF5,0x6C,
0xF6,0xA9,0x56,0xEC,0xB6,0x22,0x16,0x67,0x57,0xAF,0xF7,0xEA,0x17,0x24,0xB7,0x61,
0x38,0x8B,0x98,0xCE,0x78,0x00,0xD8,0x45,0x99,0x8D,0x39,0xC8,0xD9,0x06,0x79,0x43,
0x7A,0x86,0xDA,0xC3,0x3A,0x0D,0x9A,0x48,0xDB,0x80,0x7B,0xC5,0x9B,0x0B,0x3B,0x4E,
0xBC,0x91,0x1C,0xD4,0xFC,0x1A,0x5C,0x5F,0x1D,0x97,0xBD,0xD2,0x5D,0x1C,0xFD,0x59,
0xFE,0x9C,0x5E,0xD9,0xBE,0x17,0x1E,0x52,0x5F,0x9A,0xFF,0xDF,0x1F,0x11,0xBF,0x54,
0x00,0x00,0x61,0xB8,0xE3,0x60,0x82,0xD8,0xC6,0xC1,0xA7,0x79,0x25,0xA1,0x44,0x19,
0xAD,0x93,0xCC,0x2B,0x4E,0xF3,0x2F,0x4B,0x6B,0x52,0x0A,0xEA,0x88,0x32,0xE9,0x8A,
0x7B,0x37,0x1A,0x8F,0x98,0x57,0xF9,0xEF,0xBD,0xF6,0xDC,0x4E,0x5E,0x96,0x3F,0x2E,
0xD6,0xA4,0xB7,0x1C,0x35,0xC4,0x54,0x7C,0x10,0x65,0x71,0xDD,0xF3,0x05,0x92,0xBD,
0xF6,0x6E,0x97,0xD6,0x15,0x0E,0x74,0xB6,0x30,0xAF,0x51,0x17,0xD3,0xCF,0xB2,0x77,
0x5B,0xFD,0x3A,0x45,0xB8,0x9D,0xD9,0x25,0x9D,0x3C,0xFC,0x84,0x7E,0x5C,0x1F,0xE4,
0x8D,0x59,0xEC,0xE1,0x6E,0x39,0x0F,0x81,0x4B,0x98,0x2A,0x20,0xA8,0xF8,0xC9,0x40,
0x20,0xCA,0x41,0x72,0xC3,0xAA,0xA2,0x12,0xE6,0x0B,0x87,0xB3,0x05,0x6B,0x64,0xD3,
0xEC,0xDD,0x8D,0x65,0x0F,0xBD,0x6E,0x05,0x2A,0x1C,0x4B,0xA4,0xC9,0x7C,0xA8,0xC4,
0x41,0x4E,0x20,0xF6,0xA2,0x2E,0xC3,0x96,0x87,0x8F,0xE6,0x37,0x64,0xEF,0x05,0x57,
0x97,0xEA,0xF6,0x52,0x74,0x8A,0x15,0x32,0x51,0x2B,0x30,0x93,0xB2,0x4B,0xD3,0xF3,
0x3A,0x79,0x5B,0xC1,0xD9,0x19,0xB8,0xA1,0xFC,0xB8,0x9D,0x00,0x1F,0xD8,0x7E,0x60,
0x1A,0xB3,0x7B,0x0B,0xF9,0xD3,0x98,0x6B,0xDC,0x72,0xBD,0xCA,0x3F,0x12,0x5E,0xAA,
0xB7,0x20,0xD6,0x98,0x54,0x40,0x35,0xF8,0x71,0xE1,0x10,0x59,0x92,0x81,0xF3,0x39,
0x61,0x84,0x00,0x3C,0x82,0xE4,0xE3,0x5C,0xA7,0x45,0xC6,0xFD,0x44,0x25,0x25,0x9D,
0xCC,0x17,0xAD,0xAF,0x2F,0x77,0x4E,0xCF,0x0A,0xD6,0x6B,0x6E,0xE9,0xB6,0x88,0x0E,
0xF9,0xAB,0x98,0x13,0x1A,0xCB,0x7B,0x73,0x3F,0x6A,0x5E,0xD2,0xDC,0x0A,0xBD,0xB2,
0x54,0x38,0x35,0x80,0xB7,0x58,0xD6,0xE0,0x92,0xF9,0xF3,0x41,0x71,0x99,0x10,0x21,
0x82,0x9C,0xE3,0x24,0x61,0xFC,0x00,0x44,0x44,0x5D,0x25,0xE5,0xA7,0x3D,0xC6,0x85,
0x2F,0x0F,0x4E,0xB7,0xCC,0x6F,0xAD,0xD7,0xE9,0xCE,0x88,0x76,0x0A,0xAE,0x6B,0x16,
0x0F,0xC5,0x6E,0x7D,0xEC,0xA5,0x8D,0x1D,0xC9,0x04,0xA8,0xBC,0x2A,0x64,0x4B,0xDC,
0xA2,0x56,0xC3,0xEE,0x41,0x36,0x20,0x8E,0x64,0x97,0x05,0x2F,0x87,0xF7,0xE6,0x4F,
0x74,0xF2,0x15,0x4A,0x97,0x92,0xF6,0x2A,0xB2,0x33,0xD3,0x8B,0x51,0x53,0x30,0xEB,
0xD9,0x61,0xB8,0xD9,0x3A,0x01,0x5B,0xB9,0x1F,0xA0,0x7E,0x18,0xFC,0xC0,0x9D,0x78,
0x15,0x76,0x74,0xCE,0xF6,0x16,0x97,0xAE,0xD3,0xB7,0xB2,0x0F,0x30,0xD7,0x51,0x6F,
0xB8,0xE5,0xD9,0x5D,0x5B,0x85,0x3A,0x3D,0x7E,0x24,0x1F,0x9C,0x9D,0x44,0xFC,0xFC,
0x6E,0x41,0x0F,0xF9,0x8D,0x21,0xEC,0x99,0xA8,0x80,0xC9,0x38,0x4B,0xE0,0x2A,0x58,
0xC3,0xD2,0xA2,0x6A,0x20,0xB2,0x41,0x0A,0x05,0x13,0x64,0xAB,0xE6,0x73,0x87,0xCB,
0xE3,0x18,0x82,0xA0,0x00,0x78,0x61,0xC0,0x25,0xD9,0x44,0x61,0xC6,0xB9,0xA7,0x01,
0x4E,0x8B,0x2F,0x33,0xAD,0xEB,0xCC,0x53,0x88,0x4A,0xE9,0xF2,0x6B,0x2A,0x0A,0x92,
0x98,0x2F,0xF9,0x97,0x7B,0x4F,0x1A,0xF7,0x5E,0xEE,0x3F,0x56,0xBD,0x8E,0xDC,0x36,
0x35,0xBC,0x54,0x04,0xD6,0xDC,0xB7,0x64,0xF3,0x7D,0x92,0xC5,0x10,0x1D,0x71,0xA5,
0x00,0x00,0xD3,0x47,0xA6,0x8F,0x75,0xC8,0x6D,0x0F,0xBE,0x48,0xCB,0x80,0x18,0xC7,
0xDA,0x1E,0x09,0x59,0x7C,0x91,0xAF,0xD6,0xB7,0x11,0x64,0x56,0x11,0x9E,0xC2,0xD9,
0xB4,0x3D,0x67,0x7A,0x12,0xB2,0xC1,0xF5,0xD9,0x32,0x0A,0x75,0x7F,0xBD,0xAC,0xFA,
0x6E,0x23,0xBD,0x64,0xC8,0xAC,0x1B,0xEB,0x03,0x2C,0xD0,0x6B,0xA5,0xA3,0x76,0xE4,
0x68,0x7B,0xBB,0x3C,0xCE,0xF4,0x1D,0xB3,0x05,0x74,0xD6,0x33,0xA3,0xFB,0x70,0xBC,
0xB2,0x65,0x61,0x22,0x14,0xEA,0xC7,0xAD,0xDF,0x6A,0x0C,0x2D,0x79,0xE5,0xAA,0xA2,
0xDC,0x46,0x0F,0x01,0x7A,0xC9,0xA9,0x8E,0xB1,0x49,0x62,0x0E,0x17,0xC6,0xC4,0x81,
0x06,0x58,0xD5,0x1F,0xA0,0xD7,0x73,0x90,0x6B,0x57,0xB8,0x10,0xCD,0xD8,0x1E,0x9F,
0xD0,0xF6,0x03,0xB1,0x76,0x79,0xA5,0x3E,0xBD,0xF9,0x6E,0xBE,0x1B,0x76,0xC8,0x31,
0x0A,0xE8,0xD9,0xAF,0xAC,0x67,0x7F,0x20,0x67,0xE7,0xB4,0xA0,0xC1,0x68,0x12,0x2F,
0x64,0xCB,0xB7,0x8C,0xC2,0x44,0x11,0x03,0x09,0xC4,0xDA,0x83,0xAF,0x4B,0x7C,0x0C,
0xBE,0xD5,0x6D,0x92,0x18,0x5A,0xCB,0x1D,0xD3,0xDA,0x00,0x9D,0x75,0x55,0xA6,0x12,
0xB8,0x8D,0x6B,0xCA,0x1E,0x02,0xCD,0x45,0xD5,0x82,0x06,0xC5,0x73,0x0D,0xA0,0x4A,
0x62,0x93,0xB1,0xD4,0xC4,0x1C,0x17,0x5B,0x0F,0x9C,0xDC,0xDB,0xA9,0x13,0x7A,0x54,
0x0C,0xB0,0xDF,0xF7,0xAA,0x3F,0x79,0x78,0x61,0xBF,0xB2,0xF8,0xC7,0x30,0x14,0x77,
0xD6,0xAE,0x05,0xE9,0x70,0x21,0xA3,0x66,0xBB,0xA1,0x68,0xE6,0x1D,0x2E,0xCE,0x69,
0x81,0xFD,0x52,0xBA,0x27,0x72,0xF4,0x35,0xEC,0xF2,0x3F,0xB5,0x4A,0x7D,0x99,0x3A,
0x5B,0xE3,0x88,0xA4,0xFD,0x6C,0x2E,0x2B,0x36,0xEC,0xE5,0xAB,0x90,0x63,0x43,0x24,
0x35,0xC0,0xE6,0x87,0x93,0x4F,0x40,0x08,0x58,0xCF,0x8B,0x88,0xFE,0x40,0x2D,0x07,
0xEF,0xDE,0x3C,0x99,0x49,0x51,0x9A,0x16,0x82,0xD1,0x51,0x96,0x24,0x5E,0xF7,0x19,
0xE9,0x86,0x3A,0xC1,0x4F,0x09,0x9C,0x4E,0x84,0x89,0x57,0xCE,0x22,0x06,0xF1,0x41,
0x33,0x98,0xE0,0xDF,0x95,0x17,0x46,0x50,0x5E,0x97,0x8D,0xD0,0xF8,0x18,0x2B,0x5F,
0x5D,0xBB,0x8E,0xFC,0xFB,0x34,0x28,0x73,0x30,0xB4,0xE3,0xF3,0x96,0x3B,0x45,0x7C,
0x87,0xA5,0x54,0xE2,0x21,0x2A,0xF2,0x6D,0xEA,0xAA,0x39,0xED,0x4C,0x25,0x9F,0x62,
0x51,0x0B,0x82,0x4C,0xF7,0x84,0x24,0xC3,0x3C,0x04,0xEF,0x43,0x9A,0x8B,0x49,0xCC,
0x8B,0x15,0x58,0x52,0x2D,0x9A,0xFE,0xDD,0xE6,0x1A,0x35,0x5D,0x40,0x95,0x93,0xD2,
0xE5,0x36,0x36,0x71,0x43,0xB9,0x90,0xFE,0x88,0x39,0x5B,0x7E,0x2E,0xB6,0xFD,0xF1,
0x3F,0x28,0xEC,0x6F,0x99,0xA7,0x4A,0xE0,0x52,0x27,0x81,0x60,0xF4,0xA8,0x27,0xEF,
0x39,0x70,0xEA,0x37,0x9F,0xFF,0x4C,0xB8,0x54,0x7F,0x87,0x38,0xF2,0xF0,0x21,0xB7,
0xE3,0x6E,0x30,0x29,0x45,0xE1,0x96,0xA6,0x8E,0x61,0x5D,0x26,0x28,0xEE,0xFB,0xA9,
0x8D,0x4D,0x5E,0x0A,0x2B,0xC2,0xF8,0x85,0xE0,0x42,0x33,0x05,0x46,0xCD,0x95,0x8A,
0x57,0x53,0x84,0x14,0xF1,0xDC,0x22,0x9B,0x3A,0x5C,0xE9,0x1B,0x9C,0xD3,0x4F,0x94,
};

uint16_t crc16_lookup(uint16_t crc, const void *in_data, uint32_t len,uint16_t crc16_table[8][256]) 
{
    const uint8_t *data = in_data;
    for (uint32_t i = 0; i < len; i++) {
        crc = (crc << 8) ^ crc16_table[0][((crc >> 8) ^ data[i]) & 0x00ff];
    }

    return crc;
}

uint16_t crc16ccitt(uint16_t crc,const void *in_data,uint32_t len)
{
    return crc16_lookup(crc,in_data,len,(void *)crc16ccitt_table);
}


