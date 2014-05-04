/*
 * This code is messy, because it needs to be optimized to improve throughput.
 * It may be possible to clean it up by tweaking the HMAC/SHA256 modules, so we
 * don't have to implement optimized versions locally.
 */
#include <string.h>
#include <strong-arm/pbkdf2.h>
#include <strong-arm/hmac.h>
#include <strong-arm/sha256.h>


static void _HMAC (uint8_t *const hash, uint8_t const *const key, uint32_t klen, uint8_t const *const salt, uint32_t saltlen, uint32_t blocknum);
static void compress (uint32_t digest[static 8], uint32_t const state[static 8], uint32_t const chunk[static 16]);
static void _deserialize (uint32_t chunk[static 16], uint8_t const stream[static 64]);
static void _serialize (uint8_t stream[static 64], uint32_t const chunk[static 16]);


static const uint32_t initial_state[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};


void PBKDF2 (uint8_t *const derived_key, uint8_t const *const password, uint32_t passlen, uint8_t const *const salt, uint32_t saltlen, uint32_t iterations, uint32_t keylen)
{
	uint32_t block_count = keylen / 32;
	uint32_t sha256_state0[8] = {0};
	uint32_t sha256_state1[8] = {0};
	
	if (keylen & 31)
		block_count += 1;

	/* Precompute SHA256 states based on the password */
	{
		uint8_t padded_key[64] = {0};
		uint32_t chunk[16] = {0};

		if (passlen > 64)
			SHA256 (padded_key, password, passlen);
		else
			memmove (padded_key, password, passlen);

		_deserialize (chunk, padded_key);

		for (int i = 0; i < 16; ++i)
		{
			chunk[i] ^= 0x36363636;
		}
		compress (sha256_state0, initial_state, chunk);

		for (uint32_t i = 0; i < 16; ++i)
		{
			chunk[i] ^= 0x36363636 ^ 0x5C5C5C5C;
		}
		compress (sha256_state1, initial_state, chunk);
	}
	
	
	for (uint32_t block_num = 1; block_num <= block_count; ++block_num)
	{
		uint32_t T[16] = {0};
		uint32_t U[8] = {0};
		uint8_t block_data[32] = {0};

		/* T = HMAC (password, salt || blkNo) */
		{
			uint8_t salted[64] = {0};

			_HMAC (salted, password, passlen, salt, saltlen, block_num);
			_deserialize (T, salted);
		}
		
		/* U = T */
		for (int i = 0; i < 8; ++i)
		{
			U[i] = T[i];
		}

		/* SHA256 padding is constant on T in the iteration loop below */
		T[8] = 0x80000000;
		T[14] = 0;
		T[15] = 768;
		
		/* Iterations */
		for (uint32_t i = 1; i < iterations; ++i)
		{
			/* T = HMAC (password, T) */
			compress (T, sha256_state0, T);
			compress (T, sha256_state1, T);
			
			/* U = U ^ T */
			for (uint32_t j = 0; j < 8; ++j)
				U[j] ^= T[j];
		}

		_serialize (block_data, U);
		
		/* Write block */
		if (block_num == block_count)
		{
			memmove (derived_key + ((block_num - 1) * 32), block_data, keylen - ((block_num - 1) * 32));
			break;	/* Prevents overflow of block_num in the extreme case */
		}
		else
		{
			memmove (derived_key + ((block_num - 1) * 32), block_data, 32);
		}
	}
}


/* HMAC
 * Re-implemented locally, since the current HMAC module can't handle salt+blocknum
 */
static void _HMAC (uint8_t *const hash, uint8_t const *const key, uint32_t klen, uint8_t const *const salt, uint32_t saltlen, uint32_t blocknum)
{
	SHA256_STATE sha256_state = {0};
	uint8_t padded_key[64] = {0};
	uint8_t blocknum_str[] = {(uint8_t)(blocknum >> 24), (uint8_t)(blocknum >> 16), (uint8_t)(blocknum >> 8), (uint8_t)(blocknum)};
	
	/* If the key is longer than the SHA256 block size, hash it and pad with 0s.
	 * Otherwise, just pad with 0s. */
	if (klen > 64)
		SHA256 (padded_key, key, klen);
	else
		memmove (padded_key, key, klen);
	
	/* Inner hash */
	for (uint32_t i = 0; i < 64; ++i)
		padded_key[i] ^= 0x36;
	SHA256_partial (NULL, &sha256_state, padded_key, 64, true, false);
	SHA256_partial (NULL, &sha256_state, salt, saltlen, false, false);
	SHA256_partial (hash, &sha256_state, blocknum_str, 4, false, true);
	
	/* Outer hash */
	/* We XOR with 0x36 to remove the previous XOR */
	for (uint32_t i = 0; i < 64; ++i)
		padded_key[i] ^= 0x36 ^ 0x5C;
	SHA256_partial (NULL, &sha256_state, padded_key, 64, true, false);
	SHA256_partial (hash, &sha256_state, hash, 32, false, true);
	
	/* Result is now in (hash) */
}


/* SHA256
 * Implemented locally for optimization purposes, instead of using the existing SHA256 module.
 */
static const uint32_t kk[64] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
				0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
				0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
				0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
				0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
				0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
				0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
				0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static inline uint32_t s0 (uint32_t a);
static inline uint32_t s1 (uint32_t a);
static inline uint32_t e0 (uint32_t a);
static inline uint32_t e1 (uint32_t a);

/* gcc should optimize this to ROR instructions on at least X86 and ARM. */
#define ROR_C(x, n) (((x) >> (n)) | ((x) << (32 - (n))))


static inline uint32_t s0 (uint32_t a)
{
	uint32_t tmp1, tmp2;
	
	tmp1 = ROR_C (a, 7);
	tmp2 = ROR_C (a, 18);
	return tmp1 ^ tmp2 ^ (a >> 3);
}

static inline uint32_t s1 (uint32_t a)
{
	uint32_t tmp1, tmp2;
	
	tmp1 = ROR_C (a, 17);
	tmp2 = ROR_C (a, 19);
	return tmp1 ^ tmp2 ^ (a >> 10);
}

static inline uint32_t e0 (uint32_t a)
{
	uint32_t tmp1, tmp2;
	
	tmp1 = ROR_C (a, 2);
	tmp2 = ROR_C (a, 13);
	a = ROR_C (a, 22);
	return tmp1 ^ tmp2 ^ a;
}

static inline uint32_t e1 (uint32_t a)
{
	uint32_t tmp1, tmp2;
	
	tmp1 = ROR_C (a, 6);
	tmp2 = ROR_C (a, 11);
	a = ROR_C (a, 25);
	return tmp1 ^ tmp2 ^ a;
}


static inline uint32_t ch (uint32_t a, uint32_t b, uint32_t c)
{
	return c ^ (a & (b ^ c));
}


static inline uint32_t maj (uint32_t a, uint32_t b, uint32_t c)
{
	return (a & b) | (c & (a | b));
}


static void _deserialize (uint32_t chunk[static 16], uint8_t const stream[static 64])
{
	for (int i = 0; i < 16; ++i)
	{
		chunk[i] = stream[i<<2] << 24;
		chunk[i] |= stream[(i<<2) + 1] << 16;
		chunk[i] |= stream[(i<<2) + 2] << 8;
		chunk[i] |= stream[(i<<2) + 3];
	}
}


static void _serialize (uint8_t stream[static 64], uint32_t const chunk[static 16])
{
	for (int i = 0; i < 8; ++i)
	{
		stream[(i<<2)] = chunk[i] >> 24;
		stream[(i<<2)+1] = chunk[i] >> 16;
		stream[(i<<2)+2] = chunk[i] >> 8;
		stream[(i<<2)+3] = chunk[i];
	}
}


static void compress (uint32_t digest[static 8], uint32_t const state[static 8], uint32_t const chunk[static 16])
{
	uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
	uint32_t e = state[4], f = state[5], g = state[6], h = state[7];
	uint32_t w_data[16];

	for (uint32_t i = 0; i < 16; ++i)
		w_data[i] = chunk[i];
	
	for (uint32_t i = 0; i < 64; ++i)
	{
		uint32_t w = w_data[i&15];
		
		if (i < 48)
			w_data[i&15] = w_data[i&15] + s0 (w_data[(i+1)&15]) + w_data[(i+9)&15] + s1 (w_data[(i+14)&15]);
		
		uint32_t t2 = e0 (a) + maj (a, b, c);
		uint32_t t1 = h + e1 (e) + ch (e, f, g) + kk[i] + w;
		
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}
	
	digest[0] = state[0] + a;
	digest[1] = state[1] + b;
	digest[2] = state[2] + c;
	digest[3] = state[3] + d;
	digest[4] = state[4] + e;
	digest[5] = state[5] + f;
	digest[6] = state[6] + g;
	digest[7] = state[7] + h;
}
