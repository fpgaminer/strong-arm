#include <string.h>
#include <pbkdf2.h>
#include <hmac.h>


void PBKDF2 (uint8_t *const derived_key, uint8_t const *const password, uint32_t passlen, uint8_t const *const salt, uint32_t saltlen, uint32_t iterations, uint32_t keylen)
{
	uint32_t block_count = keylen / 32;
	uint8_t T[32], U[32];
	
	if (keylen & 31)
		block_count += 1;
	
	if (saltlen > 16)
		saltlen = 16;
	
	for (uint32_t block_num = 1; block_num <= block_count; ++block_num)
	{
		/* T = HMAC(password, salt || blkNo) */
		memmove (T, salt, saltlen);
		T[saltlen] = block_num >> 24;
		T[saltlen+1] = block_num >> 16;
		T[saltlen+2] = block_num >> 8;
		T[saltlen+3] = block_num;
		HMAC (T, password, passlen, T, saltlen+4);
		
		/* U = T */
		memmove (U, T, 32);
		
		/* Iterations */
		for (uint32_t i = 1; i < iterations; ++i)
		{
			/* T = HMAC (password, T) */
			HMAC (T, password, passlen, T, 32);
			
			/* U = U ^ T */
			for (uint32_t j = 0; j < 32; ++j)
				U[j] ^= T[j];
		}
		
		/* Write block */
		if (block_num == block_count)
		{
			memmove (derived_key + ((block_num - 1) * 32), U, keylen - ((block_num - 1) * 32));
			break;	/* Prevents overflow of block_num in the extreme case */
		}
		else
			memmove (derived_key + ((block_num - 1) * 32), U, 32);
	}
}
