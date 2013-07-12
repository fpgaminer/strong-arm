// Implements Finite Field Arithmetic
//
// TODO: This code needs to be checked for correctness and optimized.

#include <strong-arm/finite_field.h>
#include <strong-arm/random.h>
#include "ff_big.h"
#include "low_level.h"
#include "util.h"

#ifndef MAX
	#define MAX(x,y) ((x) > (y) ? (x) : (y))
#endif

static void _ff_mod (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const p);
static bool _ff_add (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b);
static bool _ff_sub (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b);
static void ff_lshift1 (FF_NUM *const out, FF_NUM const *const a);
static void ff_lshift (FF_NUM *const out, FF_NUM const *const a, uint32_t b);


////
void ff_copy (FF_NUM *const out, FF_NUM const * const a)
{
	for (int i = 0; i < 8; ++i)
		out->z[i] = a->z[i];
}

void ff_zero (FF_NUM *const out)
{
	for (int i = 0; i < 8; ++i)
		out->z[i] = 0;
}


// Internal addition; does not (mod p).
// Returns true on carry.
static bool _ff_add (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b)
{
	return _ap_add (out->z, a->z, b->z, 8);
}


// Internal subtraction; does not (mod p).
// Returns true on carry.
static bool _ff_sub (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b)
{
	return _ap_sub (out->z, a->z, b->z, 8);
}


void ff_mul (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b, FF_NUM const *const p)
{
	FF_NUM_BIG c;

	_ap_mul_256 (c.z, a->z, b->z);
	_ff_big_mod (out, &c, p);
}

int ff_compare (FF_NUM const *const a, FF_NUM const *const b)
{
	for (int i = 7; i >= 0; --i)
	{
		if (a->z[i] > b->z[i])
			return 1;	// a > b
		else if (a->z[i] < b->z[i])
			return -1;	// a < b
	}

	return 0;	// a == b
}

void ff_add (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b, FF_NUM const *const p)
{
	FF_NUM ta, tb;
	const FF_NUM *ma = a, *mb = b;
	
	// Arguments must be within the finite field
	if (ff_compare (a, p) >= 0) {
		_ff_mod (&ta, a, p);
		ma = &ta;
	}
	
	if (ff_compare (b, p) >= 0) {
		_ff_mod (&tb, b, p);
		mb = &tb;
	}
	
	if ((_ff_add (out, ma, mb)) || (ff_compare (out, p) >= 0))
		_ff_sub (out, out, p);
}

void ff_sub (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b, FF_NUM const *const p)
{
	FF_NUM ta, tb;
	const FF_NUM *ma = a, *mb = b;
	
	// Arguments must be within the finite field
	if (ff_compare (a, p) >= 0) {
		_ff_mod (&ta, a, p);
		ma = &ta;
	}
	
	if (ff_compare (b, p) >= 0) {
		_ff_mod (&tb, b, p);
		mb = &tb;
	}
	
	if (_ff_sub (out, ma, mb))
		_ff_add (out, out, p);
}

uint32_t ff_num_bits (FF_NUM const *const a)
{
	for (int i = 7; i >= 0; --i)
	{
		if (a->z[i])
			return blog2(a->z[i]) + 32 * i + 1;
	}

	return 0;
}

static void ff_lshift1 (FF_NUM *const out, FF_NUM const *const a)
{
	for (int i = 7; i > 0; --i)
		out->z[i] = (a->z[i] << 1) | (a->z[i-1] >> 31);
	out->z[0] = a->z[0] << 1;
}

static void ff_lshift (FF_NUM *const out, FF_NUM const *const a, uint32_t b)
{
	if (!b)
	{
		ff_copy (out, a);
		return;
	}

	ff_lshift1 (out, a);

	while ((--b) > 0)
		ff_lshift1 (out, out);
}

void ff_rshift1 (FF_NUM *const out, FF_NUM const *const a)
{
	for (int i = 0; i < 7; ++i)
		out->z[i] = (a->z[i] >> 1) | ((a->z[i+1] & 1) << 31);
	out->z[7] = a->z[7] >> 1;
}


// out = a / b
// rem is such that out * b + rem = a
/*void ff_div (FF_NUM *out, FF_NUM *rem, FF_NUM *a, FF_NUM *b)
{
	FF_NUM D;

	if (ff_compare (a, b) < 0)
	{
		ff_copy (rem, a);
		ff_zero (out);
		return;
	}

	const uint32_t na = ff_num_bits (a);
	const uint32_t nb = ff_num_bits (b);

	ff_copy (rem, a);
	ff_zero (out);

	// Align divisor to dividend
	// Because of compare above, na >= nb
	ff_lshift (&D, b, na-nb);

	for (int i = na - nb; i >= 0; --i)
	{
		ff_lshift1 (out, out);
		if (ff_compare (rem, &D) >= 0)
		{
			out->z[0] |= 1;
			_ff_sub (rem, rem, &D);
		}
		ff_rshift1 (&D, &D);
	}
}*/


/* Divide (a) by 58 */
void ff_div58 (FF_NUM *const out_q, FF_NUM *const out_rem, FF_NUM const *const a)
{
	FF_NUM D;
	const FF_NUM fiftyeight = {{58,0,0,0,0,0,0,0}};

	if (ff_compare (a, &fiftyeight) < 0)
	{
		ff_copy (out_rem, a);
		ff_zero (out_q);
		return;
	}

	const uint32_t na = ff_num_bits (a);

	ff_copy (out_rem, a);
	ff_zero (out_q);

	// Align divisor to dividend
	// Because of compare above, na >= nb
	ff_lshift (&D, &fiftyeight, na - 6);

	for (int i = na - 6; i >= 0; --i)
	{
		ff_lshift1 (out_q, out_q);
		if (ff_compare (out_rem, &D) >= 0)
		{
			_ff_sub (out_rem, out_rem, &D);
			out_q->z[0] |= 1;
		}
		ff_rshift1 (&D, &D);
	}
}


bool ff_is_zero (FF_NUM const *const a)
{
	for (int i = 0; i < 8; ++i)
		if (a->z[i])
			return false;
	return true;
}

bool ff_is_one (FF_NUM const *const a)
{
	for (int i = 1; i < 8; ++i)
		if (a->z[i])
			return false;
	return a->z[0] == 0x00000001;
}


void ff_inv (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const p)
{
	FF_NUM u, v;
	FF_NUM x = {{0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000}};
	FF_NUM y = {{0}};

	ff_copy (&u, a);
	ff_copy (&v, p);

	while (!ff_is_zero (&u))
	{
		while ((u.z[0] & 1) == 0)
		{
			ff_rshift1 (&u, &u);
			if ((x.z[0] & 1) == 1) {
				bool carry = _ff_add (&x, &x, p);
				ff_rshift1 (&x, &x);
				if (carry)
					x.z[7] |= 0x80000000;
				if (ff_compare (&x, p) >= 0) /// TODO: This isn't needed, (x + p) / 2 will never exceed p because x is always in the field
					_ff_sub (&x, &x, p);
			}
			else {
				ff_rshift1 (&x, &x);
			}
		}

		while ((v.z[0] & 1) == 0)
		{
			ff_rshift1 (&v, &v);
			if ((y.z[0] & 1) == 1) {
				bool carry = _ff_add (&y, &y, p);
				ff_rshift1 (&y, &y);
				if (carry)
					y.z[7] |= 0x80000000;
				if (ff_compare (&y, p) >= 0) /// TODO: This isn't needed, (y + p) / 2 will never exceed p because y is always in the field
					_ff_sub (&y, &y, p);
			}
			else {
				ff_rshift1 (&y, &y);
			}
		}

		if (ff_compare (&u, &v) >= 0)
		{
			ff_sub (&u, &u, &v, p);
			ff_sub (&x, &x, &y, p);
		}
		else
		{
			ff_sub (&v, &v, &u, p);
			ff_sub (&y, &y, &x, p);
		}
	}

	ff_copy (out, &y);
}


static void _ff_mod (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const p)
{
	FF_NUM D;

	if (ff_compare (a, p) < 0)
	{
		ff_copy (out, a);
		return;
	}

	const uint32_t na = ff_num_bits (a);
	const uint32_t nb = ff_num_bits (p);

	ff_copy (out, a);

	// Align divisor to dividend
	// Because of compare above, na >= nb
	ff_lshift (&D, p, na - nb);

	for (int i = na - nb; i >= 0; --i)
	{
		if (ff_compare (out, &D) >= 0)
			_ff_sub (out, out, &D);
		ff_rshift1 (&D, &D);
	}
}


// TODO: Verify that this really is uniform.
void ff_rand (FF_NUM *const out, FF_NUM const *const n)
{
	// Discard stale random number
	random_uint32 ();
	
	while (1)
	{
		for (int i = 0; i < 8; ++i)
			out->z[i] = random_uint32 ();
		
		if (ff_is_zero (n))
			return;
		
		if ((ff_compare (out, n) < 0) && (!ff_is_zero (out)))
			return;
	}
}


uint32_t ff_serialize (uint8_t out[static 32], FF_NUM const *const a)
{
	uint8_t *ptr = out;

	for (int i = 7; i >= 0; --i)
	{
		*(ptr++) = a->z[i] >> 24;
		*(ptr++) = a->z[i] >> 16;
		*(ptr++) = a->z[i] >> 8;
		*(ptr++) = a->z[i];
	}
	
	return 32;
}


void ff_deserialize (FF_NUM *const out, uint8_t const a[static 32])
{
	for (int i = 7; i >= 0; --i)
	{
		int j = (7 - i) * 4;

		out->z[i] = (a[j] << 24) | (a[j+1] << 16) | (a[j+2] << 8) | a[j+3];
	}	
}


uint32_t ff_der_serialize (uint8_t *const out, uint32_t maxlen, FF_NUM const *const a)
{
	uint32_t len = 0;

	if (maxlen < 35) return 0;
	out[len++] = 0x02;    // type: INTEGER
	out[len++] = 0;       // length: unknown, but no more than 33

	// Big Endian
	for (int i = 7; i >= 0; --i)
	{
		uint32_t word = a->z[i];

		for (int j = 3; j >= 0; --j)
		{
			uint8_t byte = word >> (j * 8);

			if (len == 2 && (byte == 0))
				continue;                      // Skip leading 0s
			
			if (len == 2 && (byte & 0x80))    // First bit of number must be 0 (to indicate a positive integer)
				out[len++] = 0x00;

			out[len++] = byte;
		}
	}

	if (len == 2)    // Number is 0
		out[len++] = 0;

	out[1] = len - 2;    // length minus the type and length bytes

	return len;
}
