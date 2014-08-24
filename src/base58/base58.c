#include <string.h>
#include <strong-arm/base58.h>
#include <strong-arm/sha256.h>
#include <strong-arm/finite_field.h>


void base58_encode (uint8_t *const dst, uint8_t const *const src)
{
	const uint8_t code_string[] = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	FF_NUM x = {{0}}, rem;
	uint8_t result[36];
	uint32_t result_len = 0, dst_len = 0;
	
	/* Copy src into a FF_NUM. */
	x.z[6] = src[0];
	for (uint32_t i = 0; i < 24; ++i)
	{
		x.z[5 - (i >> 2)] |= ((uint32_t)src[i + 1]) << ((3 - (i&3)) << 3);
	}
	
	/* Encode using division */
	while (!ff_is_zero (&x))
	{
		ff_div58 (&x, &rem, &x);
		result[result_len++] = code_string[rem.z[0]];
	}
	
	/* Copy leading zeros */
	for (uint32_t i = 0; i < 25; ++i)
	{
		if (src[i])
			break;
		
		dst[dst_len++] = code_string[0];
	}
	
	/* Copy reverse of result */
	for (int i = result_len - 1; i >= 0; --i)
	{
		dst[dst_len++] = result[i];
	}
	
	/* NULL Termination */
	dst[dst_len] = 0;
}

void base58check_encode (uint8_t *const dst, uint8_t version, uint8_t const *const src)
{
	uint8_t hash[32];

	memmove (dst + 1, src, 20);
	dst[0] = version;
	SHA256 (hash, dst, 21);
	SHA256 (hash, hash, 32);

	memmove (dst + 21, hash, 4);
	base58_encode (dst, dst);
}

