/* Currently, only 512-bit mode is implemented. */
#include <strong-arm/threefish.h>


/* Rotation Constants */
const uint8_t R8[8][4] = {
	{46, 36, 19, 37},
	{33, 27, 14, 42},
	{17, 49, 36, 39},
	{44, 9, 54, 56},
	{39, 30, 34, 24},
	{13, 50, 10, 17},
	{25, 29, 39, 43},
	{8, 35, 56, 22}
};

/* C240 */
const uint64_t C240 = 0x1BD11BDAA9FC1A22ull;


/* Rotate Left */
#define ROL(x, n) (((x) << (n)) | ((x) >> (64 - (n))))

/* Rotate Right */
#define ROR(x, n) (((x) >> (n)) | ((x) << (64 - (n))))


static uint64_t unpack_uint64_little (uint8_t const src[static 8])
{
	return   ((uint64_t)(src[0]) <<  0)
	       | ((uint64_t)(src[1]) <<  8)
	       | ((uint64_t)(src[2]) << 16)
	       | ((uint64_t)(src[3]) << 24)
	       | ((uint64_t)(src[4]) << 32)
	       | ((uint64_t)(src[5]) << 40)
	       | ((uint64_t)(src[6]) << 48)
	       | ((uint64_t)(src[7]) << 56);
}


static void pack_uint64_little (uint8_t dst[static 8], uint64_t src)
{
	dst[0] = (uint8_t)(src >>  0);
	dst[1] = (uint8_t)(src >>  8);
	dst[2] = (uint8_t)(src >> 16);
	dst[3] = (uint8_t)(src >> 24);
	dst[4] = (uint8_t)(src >> 32);
	dst[5] = (uint8_t)(src >> 40);
	dst[6] = (uint8_t)(src >> 48);
	dst[7] = (uint8_t)(src >> 56);
}


static void bytes_to_words (uint64_t *dst, uint8_t const *src, uint32_t count)
{
	for (; count; --count)
	{
		*(dst++) = unpack_uint64_little (src);
		src += 8;
	}
}


static void words_to_bytes (uint8_t *dst, uint64_t const *src, uint32_t count)
{
	for (; count; --count)
	{
		pack_uint64_little (dst, *src);
		src += 1;
		dst += 8;
	}
}


/* Mix function for N_w = 8 */
static void mix8 (uint64_t *y0, uint64_t *y1, uint8_t roundnum, uint8_t j, uint64_t x0, uint64_t x1)
{
	*y0 = x0 + x1;
	*y1 = ROL (x1, R8[roundnum&7][j]) ^ *y0;
}


/* Inverse Mix function for N_w = 8 */
static void unmix8 (uint64_t *x0, uint64_t *x1, uint8_t roundnum, uint8_t j, uint64_t y0, uint64_t y1)
{
	*x1 = ROR (y1 ^ y0, R8[roundnum&7][j]);
	*x0 = y0 - *x1;
}


void threefish512_encrypt_block (uint8_t ciphertext[static 64], uint8_t const key[static 64], uint8_t const tweak[static 16], uint8_t const plaintext[static 64])
{
	uint64_t k[9];
	uint64_t t[3];
	uint64_t v[8];

	bytes_to_words (k, key, 8);
	bytes_to_words (t, tweak, 2);
	bytes_to_words (v, plaintext, 8);

	/* Begin Key Schedule */
	k[8] = C240 ^ k[0] ^ k[1] ^ k[2] ^ k[3] ^ k[4] ^ k[5] ^ k[6] ^ k[7];
	t[2] = t[0] ^ t[1];

	k[5] += t[0];
	k[6] += t[1];

	for (uint8_t roundnum = 0; roundnum < 72; ++roundnum)
	{
		uint64_t extra;

		/* Add subkey */
		if ((roundnum & 3) == 0)
		{
			for (int i = 0; i < 8; ++i)
				v[i] += k[i];

			/* Update subkey */
			uint8_t s = roundnum >> 2;
			extra = k[0];
			k[0] = k[1];
			k[1] = k[2];
			k[2] = k[3];
			k[3] = k[4];
			k[4] = k[5] - t[0];
			k[5] = k[6];
			k[6] = k[7] - s + t[2];
			k[7] = k[8] + s + 1;
			k[8] = extra;

			extra = t[0];
			t[0] = t[1];
			t[1] = t[2];
			t[2] = extra;
		}

		/* Mix */
		for (int j = 0; j < 4; ++j)
			mix8 (v+(2*j), v+(2*j)+1, roundnum, j, v[2*j], v[(2*j)+1]);

		/* Permute */
		extra = v[0];
		v[0] = v[2];
		v[2] = v[4];
		v[4] = v[6];
		v[6] = extra;
		extra = v[3];
		v[3] = v[7];
		v[7] = extra;
	}

	/* Add last subkey */
	for (int i = 0; i < 8; ++i)
		v[i] += k[i];

	words_to_bytes (ciphertext, v, 8);
}


/* 
 * Decryption is the obvious inverse of encryption.  In other words, do everything backwards.
 * The only non-obvious thing here is reversing the Key Scheduling since we use a rolling key schedule
 * implementation.
 */
void threefish512_decrypt_block (uint8_t plaintext[static 64], uint8_t const key[static 64], uint8_t const tweak[static 16], uint8_t const ciphertext[static 64])
{
	uint64_t k[9];
	uint64_t t[3];
	uint64_t v[8];

	bytes_to_words (k, key, 8);
	bytes_to_words (t, tweak, 2);
	bytes_to_words (v, ciphertext, 8);

	/* Begin Key Schedule */
	k[8] = C240 ^ k[0] ^ k[1] ^ k[2] ^ k[3] ^ k[4] ^ k[5] ^ k[6] ^ k[7];
	t[2] = t[0] ^ t[1];

	k[5] += t[0];
	k[6] += t[1];
	k[7] += 18;

	/* Subtract last subkey */
	for (int i = 0; i < 8; ++i)
		v[i] -= k[i];

	for (uint8_t roundnum = 71; roundnum != 255; --roundnum)
	{
		uint64_t extra;

		/* Invert permutation */
		extra = v[2];
		v[2] = v[0];
		v[0] = v[6];
		v[6] = v[4];
		v[4] = extra;
		extra = v[3];
		v[3] = v[7];
		v[7] = extra;

		/* UnMIX */
		for (int j = 0; j < 4; ++j)
			unmix8 (v+(2*j), v+(2*j)+1, roundnum, j, v[2*j], v[(2*j)+1]);

		/* Subtract subkey */
		if ((roundnum & 3) == 0)
		{
			/* Update subkey */
			uint8_t s = roundnum >> 2;
			extra = k[8];
			k[8] = k[7] - (s+1);
			k[7] = k[6] - t[1] + s;
			k[6] = k[5];
			k[5] = k[4] + t[2];
			k[4] = k[3];
			k[3] = k[2];
			k[2] = k[1];
			k[1] = k[0];
			k[0] = extra;

			extra = t[2];
			t[2] = t[1];
			t[1] = t[0];
			t[0] = extra;

			/* Subtract */
			for (int i = 0; i < 8; ++i)
				v[i] -= k[i];
		}
	}

	words_to_bytes (plaintext, v, 8);
}
