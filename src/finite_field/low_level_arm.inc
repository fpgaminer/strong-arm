#include <stdint.h>


#ifndef MAX
	#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif


// Currently only handles 256-bit and 288-bit addition
static inline uint32_t _ap_add (uint32_t *const c, uint32_t const *const a, uint32_t const *const b, uint32_t const n)
{
	uint32_t zero = 0;

	if (n == 8)
	{
		__asm__ __volatile__ ("adds %0,%1,%2" : "=r" (c[0]) : "r" (a[0]), "r" (b[0]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[1]) : "r" (a[1]), "r" (b[1]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[2]) : "r" (a[2]), "r" (b[2]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[3]) : "r" (a[3]), "r" (b[3]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[4]) : "r" (a[4]), "r" (b[4]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[5]) : "r" (a[5]), "r" (b[5]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[6]) : "r" (a[6]), "r" (b[6]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[7]) : "r" (a[7]), "r" (b[7]));
		__asm__ __volatile__ ("adc %0,%1,%2" : "=r" (zero) : "r" (zero), "r" (zero));
	}
	else if (n == 9)
	{
		__asm__ __volatile__ ("adds %0,%1,%2" : "=r" (c[0]) : "r" (a[0]), "r" (b[0]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[1]) : "r" (a[1]), "r" (b[1]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[2]) : "r" (a[2]), "r" (b[2]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[3]) : "r" (a[3]), "r" (b[3]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[4]) : "r" (a[4]), "r" (b[4]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[5]) : "r" (a[5]), "r" (b[5]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[6]) : "r" (a[6]), "r" (b[6]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[7]) : "r" (a[7]), "r" (b[7]));
		__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (c[8]) : "r" (a[8]), "r" (b[8]));
		__asm__ __volatile__ ("adc %0,%1,%2" : "=r" (zero) : "r" (zero), "r" (zero));
	}
	else
		return 0;

	return zero ? 1 : 0;
}


// Currently only handles 256-bit, 288-bit, or 512-bit subtraction
static inline uint32_t _ap_sub (uint32_t *const c, uint32_t const *const a, uint32_t const *const b, uint32_t const n)
{
	uint32_t zero = 0;

	if (n == 8)
	{
		__asm__ __volatile__ ("subs %0,%1,%2" : "=r" (c[0]) : "r" (a[0]), "r" (b[0]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[1]) : "r" (a[1]), "r" (b[1]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[2]) : "r" (a[2]), "r" (b[2]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[3]) : "r" (a[3]), "r" (b[3]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[4]) : "r" (a[4]), "r" (b[4]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[5]) : "r" (a[5]), "r" (b[5]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[6]) : "r" (a[6]), "r" (b[6]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[7]) : "r" (a[7]), "r" (b[7]));
		__asm__ __volatile__ ("sbc %0,%1,%2" : "=r" (zero) : "r" (zero), "r" (zero));
	}
	else if (n == 9)
	{
		__asm__ __volatile__ ("subs %0,%1,%2" : "=r" (c[0]) : "r" (a[0]), "r" (b[0]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[1]) : "r" (a[1]), "r" (b[1]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[2]) : "r" (a[2]), "r" (b[2]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[3]) : "r" (a[3]), "r" (b[3]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[4]) : "r" (a[4]), "r" (b[4]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[5]) : "r" (a[5]), "r" (b[5]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[6]) : "r" (a[6]), "r" (b[6]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[7]) : "r" (a[7]), "r" (b[7]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[8]) : "r" (a[8]), "r" (b[8]));
		__asm__ __volatile__ ("sbc %0,%1,%2" : "=r" (zero) : "r" (zero), "r" (zero));
	}
	else if (n == 16)
	{
		__asm__ __volatile__ ("subs %0,%1,%2" : "=r" (c[0]) : "r" (a[0]), "r" (b[0]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[1]) : "r" (a[1]), "r" (b[1]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[2]) : "r" (a[2]), "r" (b[2]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[3]) : "r" (a[3]), "r" (b[3]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[4]) : "r" (a[4]), "r" (b[4]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[5]) : "r" (a[5]), "r" (b[5]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[6]) : "r" (a[6]), "r" (b[6]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[7]) : "r" (a[7]), "r" (b[7]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[8]) : "r" (a[8]), "r" (b[8]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[9]) : "r" (a[9]), "r" (b[9]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[10]) : "r" (a[10]), "r" (b[10]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[11]) : "r" (a[11]), "r" (b[11]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[12]) : "r" (a[12]), "r" (b[12]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[13]) : "r" (a[13]), "r" (b[13]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[14]) : "r" (a[14]), "r" (b[14]));
		__asm__ __volatile__ ("sbcs %0,%1,%2" : "=r" (c[15]) : "r" (a[15]), "r" (b[15]));
		__asm__ __volatile__ ("sbc %0,%1,%2" : "=r" (zero) : "r" (zero), "r" (zero));
	}
	else
		return 0;

	return zero ? 1 : 0;
}


static inline void _ap_mul_256 (uint32_t out[static 16], uint32_t const a[static 8], uint32_t const b[static 8])
{
	uint32_t r0 = 0, r1 = 0, r2 = 0;
	uint32_t u, v;
	uint32_t zero = 0;

	for (int k = 0; k < 15; ++k)
	{
		for (int i = MAX(0, k - 7); (i < 8) && (i <= k); ++i)
		{
			int j = k - i;

			__asm__ __volatile__ ("UMULL %0,%1,%2,%3" : "=r" (v), "=r" (u) : "r" (a[i]), "r" (b[j]));
			__asm__ __volatile__ ("adds %0,%1,%2" : "=r" (r0) : "r" (r0), "r" (v));
			__asm__ __volatile__ ("adcs %0,%1,%2" : "=r" (r1) : "r" (r1), "r" (u));
			__asm__ __volatile__ ("adc %0,%1,%2" : "=r" (r2) : "r" (r2), "r" (zero));
		}
		out[k] = r0;
		r0 = r1;
		r1 = r2;
		r2 = 0;
	}

	out[15] = r0;
}
