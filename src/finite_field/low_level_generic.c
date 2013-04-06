#include <stdint.h>


#ifndef MAX
	#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif


uint32_t _ap_add (uint32_t *const c, uint32_t const *const a, uint32_t const *const b, uint32_t const n)
{
	uint32_t carry = 0;

	for (uint32_t i = 0; i < n; ++i)
	{
		uint32_t sum = a[i] + carry;
		carry = sum < carry;
		sum += b[i];
		carry += sum < b[i];
		c[i] = sum;
	}

	return carry ? 1 : 0;
}


uint32_t _ap_sub (uint32_t *const c, uint32_t const *const a, uint32_t const *const b, uint32_t const n)
{
	uint32_t carry = 0;

	for (uint32_t i = 0; i < n; ++i)
	{
		uint32_t sum = b[i] + carry;
		carry = sum < carry;
		carry += a[i] < sum;
		c[i] = a[i] - sum;
	}

	return carry ? 1 : 0;
}


void _ap_mul_256 (uint32_t out[static 16], uint32_t const a[static 8], uint32_t const b[static 8])
{
	uint64_t r0 = 0;
	uint32_t r2 = 0;

	for (int k = 0; k < 15; ++k)
	{
		for (int i = MAX(0, k - 7); (i < 8) && (i <= k); ++i)
		{
			int j = k - i;

			uint64_t u = (uint64_t)(a[i]) * b[j];

			r0 += u;
			if (r0 < u)
				r2 += 1;
		}
		out[k] = r0 & 0xFFFFFFFF;
		r0 = (r0 >> 32) | ((uint64_t)r2 << 32);
		r2 = 0;
	}

	out[15] = r0 & 0xFFFFFFFF;
}
