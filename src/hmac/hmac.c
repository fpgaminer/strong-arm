#include <string.h>
#include <strong-arm/hmac.h>
#include <strong-arm/sha256.h>


void HMAC (uint8_t *const hash, uint8_t const *const key, uint32_t klen, uint8_t const *const msg, uint32_t mlen)
{
	HMAC_STATE state;

	HMAC_partial (hash, &state, key, klen, msg, mlen, true, true);
}


void HMAC_partial (void *hash, HMAC_STATE *const state, void const *const key, size_t klen, void const *msg, size_t mlen, bool const first, bool const last)
{
	if (first)
	{
		memset (state, 0, sizeof (HMAC_STATE));
	
		/* If the key is longer than the SHA256 block size, hash it and pad with 0s.
		 * Otherwise, just pad with 0s. */
		if (klen > 64)
			SHA256 (state->padded_key, key, klen);
		else
			memmove (state->padded_key, key, klen);
	
		/* Inner hash */
		for (uint32_t i = 0; i < 64; ++i)
			state->padded_key[i] ^= 0x36;

		SHA256_partial (0, &(state->sha256_state), state->padded_key, 64, true, false);
	}

	/* Add msg */
	SHA256_partial (hash, &(state->sha256_state), msg, mlen, false, last);

	if (!last)
		return;
	
	/* Outer hash */
	/* We XOR with 0x36 to remove the previous XOR */
	for (uint32_t i = 0; i < 64; ++i)
		state->padded_key[i] ^= 0x36 ^ 0x5C;
	SHA256_partial (NULL, &(state->sha256_state), state->padded_key, 64, true, false);
	SHA256_partial (hash, &(state->sha256_state), hash, 32, false, true);
	
	/* Result is now in (hash) */
}
