#include <string.h>
#include <minunit.h>
#include <strong-arm/threefish.h>


typedef struct
{
	uint8_t key[64];
	uint8_t tweak[16];
	uint8_t input[64];
	uint8_t output[64];
	uint32_t encrypt;
	uint32_t decrypt;
} TEST_512;


/* Test the Threefish-512 primitive */
START_TEST (test_threefish_512_block)
{
	/* Skein V1.3 Test Vectors */
	TEST_512 tests[] = {
		{
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0xb1,0xa2,0xbb,0xc6,0xef,0x60,0x25,0xbc,0x40,0xeb,0x38,0x22,0x16,0x1f,0x36,0xe3,
			 0x75,0xd1,0xbb,0x0a,0xee,0x31,0x86,0xfb,0xd1,0x9e,0x47,0xc5,0xd4,0x79,0x94,0x7b,
			 0x7b,0xc2,0xf8,0x58,0x6e,0x35,0xf0,0xcf,0xf7,0xe7,0xf0,0x30,0x84,0xb0,0xb7,0xb1,
			 0xf1,0xab,0x39,0x61,0xa5,0x80,0xa3,0xe9,0x7e,0xb4,0x1e,0xa1,0x4a,0x6d,0x7b,0xbe},
			1,
			0
		},
		{
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0xb1,0xa2,0xbb,0xc6,0xef,0x60,0x25,0xbc,0x40,0xeb,0x38,0x22,0x16,0x1f,0x36,0xe3,
			 0x75,0xd1,0xbb,0x0a,0xee,0x31,0x86,0xfb,0xd1,0x9e,0x47,0xc5,0xd4,0x79,0x94,0x7b,
			 0x7b,0xc2,0xf8,0x58,0x6e,0x35,0xf0,0xcf,0xf7,0xe7,0xf0,0x30,0x84,0xb0,0xb7,0xb1,
			 0xf1,0xab,0x39,0x61,0xa5,0x80,0xa3,0xe9,0x7e,0xb4,0x1e,0xa1,0x4a,0x6d,0x7b,0xbe},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			0,
			1
		},
		{
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
			{0xb1,0xa2,0xbb,0xc6,0xef,0x60,0x25,0xbc,0x40,0xeb,0x38,0x22,0x16,0x1f,0x36,0xe3,
			 0x75,0xd1,0xbb,0x0a,0xee,0x31,0x86,0xfb,0xd1,0x9e,0x47,0xc5,0xd4,0x79,0x94,0x7b,
			 0x7b,0xc2,0xf8,0x58,0x6e,0x35,0xf0,0xcf,0xf7,0xe7,0xf0,0x30,0x84,0xb0,0xb7,0xb1,
			 0xf1,0xab,0x39,0x61,0xa5,0x80,0xa3,0xe9,0x7e,0xb4,0x1e,0xa1,0x4a,0x6d,0x7b,0xbe},
			1024,
			1023
		},
		{
			{0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
			 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
			 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
			 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f},
			{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f},
			{0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,0xf5,0xf4,0xf3,0xf2,0xf1,0xf0,
			 0xef,0xee,0xed,0xec,0xeb,0xea,0xe9,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,
			 0xdf,0xde,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd6,0xd5,0xd4,0xd3,0xd2,0xd1,0xd0,
			 0xcf,0xce,0xcd,0xcc,0xcb,0xca,0xc9,0xc8,0xc7,0xc6,0xc5,0xc4,0xc3,0xc2,0xc1,0xc0},
			{0xe3,0x04,0x43,0x96,0x26,0xd4,0x5a,0x2c,0xb4,0x01,0xca,0xd8,0xd6,0x36,0x24,0x9a,
			 0x63,0x38,0x33,0x0e,0xb0,0x6d,0x45,0xdd,0x8b,0x36,0xb9,0x0e,0x97,0x25,0x47,0x79,
			 0x27,0x2a,0x0a,0x8d,0x99,0x46,0x35,0x04,0x78,0x44,0x20,0xea,0x18,0xc9,0xa7,0x25,
			 0xaf,0x11,0xdf,0xfe,0xa1,0x01,0x62,0x34,0x89,0x27,0x67,0x3d,0x5c,0x1c,0xaf,0x3d},
			1,
			0
		},
		{
			{0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
			 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
			 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
			 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f},
			{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f},
			{0xe3,0x04,0x43,0x96,0x26,0xd4,0x5a,0x2c,0xb4,0x01,0xca,0xd8,0xd6,0x36,0x24,0x9a,
			 0x63,0x38,0x33,0x0e,0xb0,0x6d,0x45,0xdd,0x8b,0x36,0xb9,0x0e,0x97,0x25,0x47,0x79,
			 0x27,0x2a,0x0a,0x8d,0x99,0x46,0x35,0x04,0x78,0x44,0x20,0xea,0x18,0xc9,0xa7,0x25,
			 0xaf,0x11,0xdf,0xfe,0xa1,0x01,0x62,0x34,0x89,0x27,0x67,0x3d,0x5c,0x1c,0xaf,0x3d},
			{0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,0xf5,0xf4,0xf3,0xf2,0xf1,0xf0,
			 0xef,0xee,0xed,0xec,0xeb,0xea,0xe9,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,
			 0xdf,0xde,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd6,0xd5,0xd4,0xd3,0xd2,0xd1,0xd0,
			 0xcf,0xce,0xcd,0xcc,0xcb,0xca,0xc9,0xc8,0xc7,0xc6,0xc5,0xc4,0xc3,0xc2,0xc1,0xc0},
			0,
			1
		},
		{
			{0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,
			 0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,
			 0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
			 0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e,0x4f},
			{0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f},
			{0xff,0xfe,0xfd,0xfc,0xfb,0xfa,0xf9,0xf8,0xf7,0xf6,0xf5,0xf4,0xf3,0xf2,0xf1,0xf0,
			 0xef,0xee,0xed,0xec,0xeb,0xea,0xe9,0xe8,0xe7,0xe6,0xe5,0xe4,0xe3,0xe2,0xe1,0xe0,
			 0xdf,0xde,0xdd,0xdc,0xdb,0xda,0xd9,0xd8,0xd7,0xd6,0xd5,0xd4,0xd3,0xd2,0xd1,0xd0,
			 0xcf,0xce,0xcd,0xcc,0xcb,0xca,0xc9,0xc8,0xc7,0xc6,0xc5,0xc4,0xc3,0xc2,0xc1,0xc0},
			{0xe3,0x04,0x43,0x96,0x26,0xd4,0x5a,0x2c,0xb4,0x01,0xca,0xd8,0xd6,0x36,0x24,0x9a,
			 0x63,0x38,0x33,0x0e,0xb0,0x6d,0x45,0xdd,0x8b,0x36,0xb9,0x0e,0x97,0x25,0x47,0x79,
			 0x27,0x2a,0x0a,0x8d,0x99,0x46,0x35,0x04,0x78,0x44,0x20,0xea,0x18,0xc9,0xa7,0x25,
			 0xaf,0x11,0xdf,0xfe,0xa1,0x01,0x62,0x34,0x89,0x27,0x67,0x3d,0x5c,0x1c,0xaf,0x3d},
			1024,
			1023
		},
		{
			{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
			{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
			{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
			 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
			{0xc9,0x61,0x6e,0x31,0x7a,0xe3,0x71,0xd2,0xe5,0xeb,0x2b,0xd2,0xc6,0x4c,0x53,0xac,
			 0x07,0x22,0x02,0x0c,0xb9,0x97,0xac,0x16,0x6e,0x43,0x79,0x97,0x88,0xc9,0x2b,0x86,
			 0x2a,0x2e,0xe1,0x04,0x85,0x14,0x67,0xd5,0xc2,0x1f,0xdd,0x80,0xaa,0x51,0x5d,0x97,
			 0xe2,0xfb,0x58,0x97,0xfe,0xff,0x0e,0x56,0x25,0x0d,0x8a,0x70,0xe0,0xf0,0xc0,0xd0},
			1024,
			1023
		},
		{
			{0x00,0xe1,0x95,0x18,0x2a,0xce,0x36,0x25,0xb6,0x1c,0xa9,0x53,0x53,0xc6,0x21,0xd8,
			 0x8b,0xa5,0x98,0xb9,0xb0,0x7f,0x74,0x29,0x48,0x94,0x6f,0x08,0x20,0x82,0x14,0xc5,
			 0x52,0xea,0xea,0x37,0xb8,0xee,0x59,0xcb,0x2d,0x4c,0x23,0xcc,0x81,0xb2,0x45,0x95,
			 0x6a,0x0e,0x7c,0x02,0x1b,0x04,0x83,0x42,0xb1,0x0a,0xce,0x0d,0x84,0x11,0x4c,0x99},
			{0x7f,0x35,0x01,0x29,0x06,0xa5,0x2e,0x5e,0xfe,0x67,0xeb,0xb3,0xfd,0x05,0x99,0x32},
			{0xa4,0x41,0x9b,0xc9,0xe1,0xe6,0x43,0x29,0x66,0x3d,0xdb,0x61,0xcd,0x39,0x74,0x95,
			 0x99,0x48,0xe9,0xac,0xb0,0x84,0xf5,0x6c,0xdd,0x13,0x1f,0xd7,0xc2,0xcb,0x49,0x8d,
			 0xcf,0x3f,0x0d,0x09,0x9f,0x1d,0xe5,0x99,0xe3,0xd1,0x00,0x1d,0xef,0xee,0xe5,0xf2,
			 0x38,0xfb,0xc5,0x75,0x13,0x2d,0x8d,0x37,0xea,0x18,0xb6,0x34,0xf0,0xcf,0xdb,0x4a},
			{0x8b,0x07,0xf2,0x58,0xea,0xd3,0xdb,0x36,0x63,0x39,0x47,0x83,0xda,0x0d,0x48,0xbb,
			 0x60,0xdd,0x1b,0xbb,0x95,0xc4,0x3a,0x34,0x04,0x8c,0x69,0x64,0x45,0x5d,0x6b,0x07,
			 0x42,0x92,0xda,0x98,0x75,0x02,0xee,0xfe,0x84,0x50,0x97,0x76,0xc4,0x2a,0x6b,0x6b,
			 0x7c,0xe0,0xca,0x84,0xee,0xea,0x34,0x2f,0xaa,0x70,0xf4,0x60,0xec,0x2a,0xd0,0x66},
			1024,
			1023
		},
		{
			{0x40,0x4a,0x93,0xfb,0xcc,0xa5,0x45,0x9c,0x2d,0x30,0xa3,0x35,0x3a,0x31,0xdc,0xa7,
			 0x85,0x5f,0x57,0x24,0x10,0xe0,0x81,0x8b,0x0b,0x43,0x7a,0xf3,0xa2,0x55,0x83,0x2a,
			 0x13,0x93,0x61,0x9b,0x8d,0x88,0xea,0xf2,0x26,0xac,0xdd,0xfb,0x6f,0xdc,0x47,0x44,
			 0x08,0x24,0x51,0x83,0xbb,0x5a,0xd9,0xc4,0xaf,0x36,0x17,0x94,0x5f,0x48,0xa7,0xea},
			{0x1d,0xbf,0xbe,0xe1,0x28,0x98,0xb7,0xe1,0x02,0xa1,0x47,0x1f,0xec,0xbd,0x8d,0x2e},
			{0x62,0xea,0x0a,0x7a,0x6d,0x1d,0xf9,0x85,0x59,0x33,0x44,0xdf,0xc7,0xeb,0x65,0xbc,
			 0x70,0x23,0x1a,0x92,0xc2,0x97,0xbf,0x78,0x4e,0x8c,0x09,0xa3,0x29,0x57,0x9c,0x4e,
			 0x26,0x77,0x2e,0x25,0x9a,0xac,0x8e,0x4c,0xbf,0x06,0x71,0xf9,0xf4,0xdb,0xbb,0x83,
			 0xb8,0x0b,0x88,0xd5,0x4c,0xa3,0xf7,0xc4,0x25,0xe8,0x6d,0x83,0x29,0x05,0xb9,0xb4},
			{0x1c,0xd0,0xd7,0x8b,0x5b,0xfe,0x7c,0x47,0xb3,0x8d,0xce,0xe5,0x5a,0x58,0xa0,0x3d,
			 0x6c,0x29,0xbe,0x55,0xd9,0xc9,0xdf,0xad,0x75,0x33,0x1e,0x14,0xba,0x0b,0x98,0xb1,
			 0xca,0xca,0x02,0x6b,0x99,0x2f,0x3c,0xf0,0x05,0x36,0x70,0x3e,0x46,0x98,0x59,0xa7,
			 0xf2,0xa6,0x11,0x81,0xfc,0xb3,0x67,0x0d,0xaf,0xa5,0x8e,0x83,0x47,0x99,0x13,0x9e},
			1021,
			0
		},
		{
			{0x28,0x38,0x55,0x20,0x4c,0x25,0x07,0x23,0xa7,0x05,0x7e,0x5d,0x3e,0x9a,0x68,0x5f,
			 0xcf,0x0c,0x28,0xbc,0xb9,0xd7,0x0b,0xc8,0x9b,0xe9,0x22,0xad,0x34,0x2b,0x31,0xba,
			 0x14,0x06,0x17,0xa7,0x7b,0xa1,0x02,0xc6,0x8c,0x76,0xbb,0x4c,0x5f,0x86,0xa9,0x24,
			 0x33,0xae,0x3b,0xa0,0xcd,0xc4,0x2b,0x58,0xdb,0x81,0x32,0xd9,0xc0,0x5b,0x94,0x4b},
			{0x7c,0xa9,0xca,0xbc,0x15,0xca,0x26,0xf8,0xf5,0x74,0x9f,0x78,0xac,0x04,0x22,0x13},
			{0x15,0x47,0x68,0xdf,0x39,0xab,0x02,0x1a,0xdf,0xa4,0xdb,0xd3,0x6b,0xf7,0xdd,0x25,
			 0x3f,0x22,0x7e,0x67,0x3e,0xcc,0xf7,0xfb,0x61,0xda,0xdd,0xc4,0x0e,0x35,0x2f,0xe7,
			 0xaf,0x57,0xf1,0x6e,0x2d,0x87,0xba,0x2f,0x5e,0x6b,0x18,0x81,0x1b,0xeb,0xfb,0x1c,
			 0x93,0x52,0xd0,0x4e,0x38,0x7f,0xcb,0x70,0xd1,0x1c,0xe7,0x50,0x17,0x61,0xf5,0xe6},
			{0x06,0xe3,0x11,0x28,0x52,0xbd,0x22,0xcd,0xf1,0x1c,0x75,0xb5,0x8c,0xb4,0xa0,0xab,
			 0xa0,0x53,0x26,0x12,0x87,0x9d,0xa2,0x88,0x9c,0xbf,0x7e,0xa1,0xa5,0xf4,0x68,0x7d,
			 0x16,0xd2,0xd2,0xb1,0x7c,0x66,0xf0,0x01,0xe1,0x93,0xeb,0x5d,0x98,0x05,0xde,0x92,
			 0x0a,0xee,0x77,0x44,0x26,0xb4,0x1d,0xf6,0xbb,0x9f,0x0c,0x1f,0x77,0xa6,0x03,0x0d},
			0,
			1021
		},
	};

	uint8_t input1[64] = {0x6e,0x79,0x9c,0x72,0x35,0x14,0x3c,0xb2,0x88,0xe3,0x4b,0x35,0x3a,0x24,0xa1,0x70,
	                      0xd5,0xfc,0x7d,0x54,0xa2,0x7b,0x00,0x9e,0xbd,0xd1,0x4e,0x6b,0xc1,0x98,0xa6,0xc0,
	                      0xaf,0xbb,0x72,0x4d,0xe6,0x8a,0x9d,0xc4,0xa4,0x8a,0xc0,0x93,0x17,0x46,0xe4,0x11,
	                      0x56,0x77,0x11,0x9a,0x87,0x41,0x03,0xa0,0xc4,0x75,0x46,0x64,0x18,0x7a,0x02,0x0e};
	uint8_t output1[64] = {0x32,0xbc,0xa7,0xc0,0x52,0x57,0x20,0xbd,0x91,0x04,0xbf,0x51,0x3a,0x22,0xe7,0x1d,
	                       0x31,0x0b,0xe8,0x84,0xfc,0xb3,0xe5,0x4e,0x7d,0xad,0xf0,0x7f,0xca,0xbd,0x4c,0xd0,
	                       0x0e,0xf2,0x57,0xd4,0x20,0x9a,0x69,0xff,0x7d,0x2f,0xc2,0x8e,0x64,0x02,0xe3,0x52,
	                       0x14,0x3c,0x91,0x42,0x08,0x92,0x96,0x00,0xac,0x20,0x6d,0x9b,0x81,0x2a,0x48,0x7d};
	uint8_t output2[64] = {0x8d,0x0a,0xb4,0xe5,0x27,0x28,0x3a,0x9a,0x34,0x00,0x30,0x22,0xc7,0xf9,0x2f,0x00,
	                       0x37,0x03,0x31,0xdb,0x51,0xc9,0x6c,0x7e,0xc0,0xbf,0x9a,0xad,0xad,0x51,0x35,0x33,
	                       0x36,0xd3,0xea,0xe3,0x2f,0x42,0x84,0x67,0xb3,0x66,0xbc,0x52,0xf3,0x7c,0x04,0x20,
	                       0xb6,0x9a,0x19,0xd2,0x41,0x7b,0xdb,0xcb,0x18,0x2b,0x5b,0xb0,0x96,0xb7,0x47,0x06};

	uint8_t output[65] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	                      0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	                      0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	                      0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	                      0x55};

	/* Process Test Vectors */
	for (unsigned int test_i = 0; test_i < sizeof (tests) / sizeof (TEST_512); ++test_i)
	{
		TEST_512 test = tests[test_i];

		memmove (output, test.input, 64);

		for (unsigned int i = 0; i < test.encrypt; ++i)
			threefish512_encrypt_block (output, test.key, test.tweak, output);
		
		for (unsigned int i = 0; i < test.decrypt; ++i)
			threefish512_decrypt_block (output, test.key, test.tweak, output);

		mu_assert (memcmp (output, test.output, 64) == 0, "threefish512_*crypt_block should correctly process the test vectors.");
	}

	/* Test that we can call threefish512_* with the same array for all parameters */
	memmove (output, input1, 64);
	threefish512_encrypt_block (output, output, output, output);
	mu_assert (memcmp (output, output1, 64) == 0, "threefish512_encrypt_block should work even if the same array is passed to all parameters.");

	memmove (output, input1, 64);
	threefish512_decrypt_block (output, output, output, output);
	mu_assert (memcmp (output, output2, 64) == 0, "threefish512_decrypt_block should work even if the same array is passed to all parameters.");

	/* Buffer overflow check */
	mu_assert (output[64] == 0x55, "threefish512_encrypt_block should not write more than 64 bytes to the destination array.");
}
END_TEST


char *test_threefish (void)
{
	mu_run_test (test_threefish_512_block);

	return 0;
}
