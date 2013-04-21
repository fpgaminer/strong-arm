#include <string.h>
#include <strong-arm/hmac.h>
#include <strong-arm/sha256.h>


void HMAC (uint8_t *const hash, uint8_t const *const key, uint32_t klen, uint8_t const *const msg, uint32_t mlen)
{
	uint8_t padded_key[64] = {0};
	
	/* If the key is longer than the SHA256 block size, hash it and pad with 0s.
	 * Otherwise, just pad with 0s. */
	if (klen > 64)
		SHA256 (padded_key, key, klen);
	else
		memmove (padded_key, key, klen);
	
	/* Inner hash */
	for (uint32_t i = 0; i < 64; ++i)
		padded_key[i] ^= 0x36;
	SHA256_partial (0, padded_key, 64, true, false);
	SHA256_partial (hash, msg, mlen, false, true);
	
	/* Outer hash */
	/* We XOR with 0x36 to remove the previous XOR */
	for (uint32_t i = 0; i < 64; ++i)
		padded_key[i] ^= 0x36 ^ 0x5C;
	SHA256_partial (NULL, padded_key, 64, true, false);
	SHA256_partial (hash, hash, 32, false, true);
	
	/* Result is now in (hash) */
}
