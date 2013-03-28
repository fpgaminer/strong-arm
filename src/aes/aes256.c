/*
 * Inspired by and copied in part from LibTomCrypt:
 * https://github.com/libtom/libtomcrypt
 *
 */
#include <string.h>
#include <aes.h>
#include "aes256_tables.h"


static inline __attribute__((always_inline)) uint32_t ror (uint32_t a, uint32_t const i)
{
	uint32_t tmp;
#ifdef __arm__
	__asm__ ("ror %[Rd],%[Rm],%[Is]" : [Rd] "=r" (tmp) : [Rm] "r" (a), [Is] "i" (i));
#elif __i386__
	__asm__ ("movl %[Rm],%[Rd]; rorl %[Is],%[Rd]" : [Rd] "=r" (tmp) : [Rm] "r" (a), [Is] "i" (i));
#else
	#error Target architecture must be ARM or X86
#endif
	return tmp;
}


static uint32_t setup_mix (uint32_t temp)
{
	return (Te4[(temp >> 16) & 0xFF] & 0xFF000000) ^
               (Te4[(temp >> 8) & 0xFF] & 0x00FF0000) ^
	       (Te4[temp & 0xFF] & 0x0000FF00) ^
	       (Te4[(temp >> 24) & 0xFF] & 0x000000FF);
}


static uint32_t final_mix (uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	return (Te4[a] & 0xFF000000) ^
               (Te4[b] & 0x00FF0000) ^
	       (Te4[c] & 0x0000FF00) ^
	       (Te4[d] & 0x000000FF);
}


static uint32_t load_big_endian (uint8_t const buf[static 4])
{
	return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}


static void save_big_endian (uint8_t buf[static 4], uint32_t word)
{
	buf[0] = word >> 24;
	buf[1] = word >> 16;
	buf[2] = word >> 8;
	buf[3] = word;
}


void aes256_encrypt_block (uint8_t ciphertext[static 16], uint8_t const key[static 32], uint8_t const plaintext[static 16])
{
	uint32_t eK[60];
	uint32_t t0, t1, t2, t3;

	/* Key Scheduling */
	for (int i = 0; i < 8; ++i)
		eK[i] = load_big_endian (key + i * 4);

	for (int i = 0; i < 64-8; i += 8)
	{
		eK[i+8] = eK[i] ^ setup_mix (eK[i+7]) ^ rcon[i/8];
		eK[i+9] = eK[i+1] ^ eK[i+8];
		eK[i+10] = eK[i+2] ^ eK[i+9];
		eK[i+11] = eK[i+3] ^ eK[i+10];

		if (i == 48)
			break;

		eK[i+12] = eK[i+4] ^ setup_mix (ror (eK[i+11], 8));
		eK[i+13] = eK[i+5] ^ eK[i+12];
		eK[i+14] = eK[i+6] ^ eK[i+13];
		eK[i+15] = eK[i+7] ^ eK[i+14];
	}

	/* Encryption */
	uint32_t s0 = load_big_endian (plaintext);
	uint32_t s1 = load_big_endian (plaintext + 4);
	uint32_t s2 = load_big_endian (plaintext + 8);
	uint32_t s3 = load_big_endian (plaintext + 12);

	s0 ^= eK[0];
	s1 ^= eK[1];
	s2 ^= eK[2];
	s3 ^= eK[3];

	for (int r = 1; ; ++r)
	{
		t0 = TE0[(s0 >> 24) & 0xFF] ^
		     ror (TE0[(s1 >> 16) & 0xFF], 8) ^
		     ror (TE0[(s2 >> 8) & 0xFF], 16) ^
		     ror (TE0[s3 & 0xFF], 24) ^
		     eK[r*4];

		t1 = TE0[(s1 >> 24) & 0xFF] ^
		     ror (TE0[(s2 >> 16) & 0xFF], 8) ^
		     ror (TE0[(s3 >> 8) & 0xFF], 16) ^
		     ror (TE0[s0 & 0xFF], 24) ^
		     eK[r*4 + 1];

		t2 = TE0[(s2 >> 24) & 0xFF] ^
		     ror (TE0[(s3 >> 16) & 0xFF], 8) ^
		     ror (TE0[(s0 >> 8) & 0xFF], 16) ^
		     ror (TE0[s1 & 0xFF], 24) ^
		     eK[r*4 + 2];

		t3 = TE0[(s3 >> 24) & 0xFF] ^
		     ror (TE0[(s0 >> 16) & 0xFF], 8) ^
		     ror (TE0[(s1 >> 8) & 0xFF], 16) ^
		     ror (TE0[s2 & 0xFF], 24) ^
		     eK[r*4 + 3];

		if (r == 13)
			break;

		s0 = t0; s1 = t1; s2 = t2; s3 = t3;
	}


	s0 = final_mix (t0 >> 24, t1 >> 16, t2 >> 8, t3) ^ eK[56];
	save_big_endian (ciphertext, s0);

	s1 = final_mix (t1 >> 24, t2 >> 16, t3 >> 8, t0) ^ eK[57];
	save_big_endian (ciphertext + 4, s1);

	s2 = final_mix (t2 >> 24, t3 >> 16, t0 >> 8, t1) ^ eK[58];
	save_big_endian (ciphertext + 8, s2);

	s3 = final_mix (t3 >> 24, t0 >> 16, t1 >> 8, t2) ^ eK[59];
	save_big_endian (ciphertext + 12, s3);
}


/* Increment a Big-Endian counter */
static void increment_counter (uint8_t counter[static 16])
{
	for (int i = 15; i >= 0; --i)
	{
		if ((counter[i] += 1) != 0)
			break;
	}
}


void aes256_crypt_ctr (uint8_t *ciphertext, uint8_t const key[static 32], uint8_t const iv[static 16], uint8_t const *plaintext, uint32_t len)
{
	uint8_t saved_key[32] = {0};
	uint8_t counter[16] = {0};
	uint8_t cipherblock[16] = {0};

	/* Initialize */
	memmove (counter, iv, 16);
	memmove (saved_key, key, 32);

	while (len > 0)
	{
		aes256_encrypt_block (cipherblock, saved_key, counter);
		increment_counter (counter);

		for (int i = 0; i < 16; ++i)
		{
			*ciphertext = (*plaintext) ^ cipherblock[i];
			++ciphertext;
			++plaintext;
			--len;

			if (len == 0)
				break;
		}
	}
}
