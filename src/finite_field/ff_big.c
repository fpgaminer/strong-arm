#include "ff_big.h"
#include "util.h"

/* */
static void ff_big_copy (FF_NUM_BIG *out, FF_NUM_BIG const *const a);

/* It is assumed that the argument is < 2^256 */
static void ff_big_to_small (FF_NUM *const out, FF_NUM_BIG const *const a);

/* Returns 1 if a > b
 * Returns -1 if a < b
 * Returns 0 if a == b */
static int ff_big_compare (FF_NUM_BIG const *const a, FF_NUM_BIG const *const b);

/* */
static uint32_t ff_big_num_bits (FF_NUM_BIG const *const a);

/* */
static void ff_big_lshift1 (FF_NUM_BIG *const out, FF_NUM_BIG const *const a);

/* */
static void ff_big_lshift (FF_NUM_BIG *const out, FF_NUM_BIG const *const a, uint32_t b);

/* */
static void ff_big_rshift1 (FF_NUM_BIG *const out, FF_NUM_BIG const *const a);

/* */
static void _ff_big_sub (FF_NUM_BIG *const out, FF_NUM_BIG const *const a, FF_NUM_BIG const *const b);



/* *** */
static void ff_big_copy (FF_NUM_BIG *out, FF_NUM_BIG const *const a)
{
	for (int i = 0; i < 16; ++i)
	{
		out->z[i] = a->z[i];
	}
}

static void ff_big_to_small (FF_NUM *const out, FF_NUM_BIG const *const a)
{
	for (int i = 0; i < 8; ++i)
	{
		out->z[i] = a->z[i];
	}
}

static void _ff_big_sub (FF_NUM_BIG *const out, FF_NUM_BIG const *const a, FF_NUM_BIG const *const b)
{
#ifdef __arm__
	__asm__("subs %0,%1,%2" : "=r" (out->z[0]) : "r" (a->z[0]), "r" (b->z[0]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[1]) : "r" (a->z[1]), "r" (b->z[1]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[2]) : "r" (a->z[2]), "r" (b->z[2]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[3]) : "r" (a->z[3]), "r" (b->z[3]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[4]) : "r" (a->z[4]), "r" (b->z[4]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[5]) : "r" (a->z[5]), "r" (b->z[5]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[6]) : "r" (a->z[6]), "r" (b->z[6]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[7]) : "r" (a->z[7]), "r" (b->z[7]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[8]) : "r" (a->z[8]), "r" (b->z[8]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[9]) : "r" (a->z[9]), "r" (b->z[9]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[10]) : "r" (a->z[10]), "r" (b->z[10]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[11]) : "r" (a->z[11]), "r" (b->z[11]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[12]) : "r" (a->z[12]), "r" (b->z[12]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[13]) : "r" (a->z[13]), "r" (b->z[13]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[14]) : "r" (a->z[14]), "r" (b->z[14]));
	__asm__("sbcs %0,%1,%2" : "=r" (out->z[15]) : "r" (a->z[15]), "r" (b->z[15]));
#elif __i386__
	*out = *a;
	__asm__("subl %1, %0;" : "=m" (out->z[0]) : "r" (b->z[0]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[1]) : "r" (b->z[1]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[2]) : "r" (b->z[2]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[3]) : "r" (b->z[3]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[4]) : "r" (b->z[4]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[5]) : "r" (b->z[5]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[6]) : "r" (b->z[6]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[7]) : "r" (b->z[7]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[8]) : "r" (b->z[8]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[9]) : "r" (b->z[9]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[10]) : "r" (b->z[10]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[11]) : "r" (b->z[11]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[12]) : "r" (b->z[12]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[13]) : "r" (b->z[13]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[14]) : "r" (b->z[14]));
	__asm__("sbbl %1, %0;" : "=m" (out->z[15]) : "r" (b->z[15]));
#else
	#error Must compile for ARM or X86
#endif
}

void _ff_big_mod (FF_NUM *const out, FF_NUM_BIG const *const a, FF_NUM const *const p)
{
	FF_NUM_BIG D;
	FF_NUM_BIG rem;
	FF_NUM_BIG m = {{0}};
	
	for (int i = 0; i < 8; ++i)
	{
		m.z[i] = p->z[i];
	}

	if (ff_big_compare (a, &m) < 0)
	{
		ff_big_to_small (out, a);
		return;
	}

	const uint32_t na = ff_big_num_bits (a);
	const uint32_t nb = ff_big_num_bits (&m);

	ff_big_copy (&rem, a);

	/* Align divisor to dividend
	 * Because of compare above, na >= nb */
	ff_big_lshift (&D, &m, na-nb);

	for (uint32_t i = na - nb; true; --i)
	{
		if (ff_big_compare (&rem, &D) >= 0)
			_ff_big_sub (&rem, &rem, &D);
		ff_big_rshift1 (&D, &D);
		
		if (i == 0)
			break;
	}

	ff_big_to_small (out, &rem);
}

static int ff_big_compare (FF_NUM_BIG const *const a, FF_NUM_BIG const *const b)
{
	for (int i = 15; i >= 0; --i)
	{
		if (a->z[i] > b->z[i])
			return 1;		// a > b
		else if (a->z[i] < b->z[i])
			return -1;	// a < b
	}

	return 0;		// a == b
}

static uint32_t ff_big_num_bits (FF_NUM_BIG const *const a)
{
	for (int i = 15; i >= 0; --i)
	{
		if (a->z[i])
			return blog2(a->z[i]) + 32 * i + 1;
	}

	return 0;
}

static void ff_big_lshift1 (FF_NUM_BIG *const out, FF_NUM_BIG const *const a)
{
	for (int i = 15; i > 0; --i)
		out->z[i] = (a->z[i] << 1) | (a->z[i-1] >> 31);
	out->z[0] = a->z[0] << 1;
}

static void ff_big_lshift (FF_NUM_BIG *const out, FF_NUM_BIG const *const a, uint32_t b)
{
	if (!b)
	{
		ff_big_copy (out, a);
		return;
	}

	ff_big_lshift1 (out, a);

	for (int i = 1; i < b; ++i)
		ff_big_lshift1 (out, out);
}

static void ff_big_rshift1 (FF_NUM_BIG *const out, FF_NUM_BIG const *const a)
{
	for (int i = 0; i < 15; ++i)
		out->z[i] = (a->z[i] >> 1) | ((a->z[i+1] & 1) << 31);
	out->z[15] = a->z[15] >> 1;
}

