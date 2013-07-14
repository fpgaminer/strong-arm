#include "ff_big.h"
#include "util.h"
#include "low_level.h"

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

/* Optimized specifically for secp256k1's finite field. */
static void _ff_big_mod_secp256k1 (FF_NUM *const out, FF_NUM_BIG const *const a);



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
	_ap_sub (out->z, a->z, b->z, 16);
}

void _ff_big_mod (FF_NUM *const out, FF_NUM_BIG const *const a, FF_NUM const *const p)
{
	FF_NUM_BIG D;
	FF_NUM_BIG rem;
	FF_NUM_BIG m = {{0}};
	const FF_NUM ec_p = {{0xFFFFFC2F, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};

	// Optimize for secp256k1's prime
	if (ff_compare (p, &ec_p) == 0)
	{
		_ff_big_mod_secp256k1 (out, a);
		return;
	}
	
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


static void _ap_shift_helper (uint32_t c[static 9], uint32_t a[static 9], uint32_t const s)
{
	if (s == 32)
		c[0] = 0;
	else
		c[0] = a[0] << s;

	for (int i = 1; i < 8; ++i)
	{
		c[i] = a[i-1] >> (32 - s);
		if (s < 32)
			c[i] |= a[i] << s;
	}

	c[8] = 0;
}


static void _ff_big_mod_secp256k1 (FF_NUM *const out, FF_NUM_BIG const *const a)
{
	const uint32_t p[9] = {0xFFFFFC2F, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000};
	uint32_t c1[9] = {0};
	uint32_t w[9] = {0};
	uint32_t s[9] = {0};

	// c1 = a >> 256
	for (int i = 0; i < 8; ++i)
	{
		c1[i] = a->z[i + 8];
	}

	uint32_t k = (c1[7] >> 28) + (c1[7] >> 26);
	k += (c1[7] >> 25);
	k += (c1[7] >> 24);
	k += (c1[7] >> 23);

	uint64_t s1 = (uint64_t)k + c1[7];
	uint64_t k11 = (s1 << 2) + (s1 << 1) + s1;
	uint64_t k12 = k11 << 7;
	uint64_t k13 = (s1 << 4) + s1;
	uint64_t k14 = (s1 << 6) + k13;
	uint64_t k12_k14 = k12 + k14;

	// s = c0 + c1 + (s1 << 32) + k12_k14 + w1 + w2 + w3 + w4 + w5 + w6
	for (int i = 0; i < 8; ++i)
	{
		s[i] = a->z[i];
	}

	_ap_add (s, s, c1, 9);

	w[1] = s1;
	w[2] = s1 >> 32;
	_ap_add (s, s, w, 9);

	w[0] = k12_k14;
	w[1] = k12_k14 >> 32;
	w[2] = 0;
	_ap_add (s, s, w, 9);

	_ap_shift_helper (w, c1, 32);
	_ap_add (s, s, w, 9);

	_ap_shift_helper (w, c1, 9);
	_ap_add (s, s, w, 9);

	_ap_shift_helper (w, c1, 8);
	_ap_add (s, s, w, 9);

	_ap_shift_helper (w, c1, 7);
	_ap_add (s, s, w, 9);

	_ap_shift_helper (w, c1, 6);
	_ap_add (s, s, w, 9);

	_ap_shift_helper (w, c1, 4);
	_ap_add (s, s, w, 9);

	// while (s >= p) s -= p
	// At most, p will need to be subtracted 8 times.
	while (true)
	{
		if (_ap_sub (w, s, p, 9))
		{
			for (int i = 0; i < 8; ++i)
			{
				out->z[i] = s[i];
			}

			break;
		}

		if (_ap_sub (s, w, p, 9))
		{
			for (int i = 0; i < 8; ++i)
			{
				out->z[i] = w[i];
			}

			break;
		}
	}
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

	while ((--b) > 0)
		ff_big_lshift1 (out, out);
}

static void ff_big_rshift1 (FF_NUM_BIG *const out, FF_NUM_BIG const *const a)
{
	for (int i = 0; i < 15; ++i)
		out->z[i] = (a->z[i] >> 1) | ((a->z[i+1] & 1) << 31);
	out->z[15] = a->z[15] >> 1;
}

