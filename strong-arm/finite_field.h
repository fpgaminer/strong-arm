/* 
 * Implements Fixed Size Finite Field Arithmetic.
 * The primary type is FF_NUM, which can store any number from 0 to 2^256 - 1.
 *
 * Data is stored in FF_NUM as an array of 8 32-bit unsigned words. Least
 * significant word is first. 2^255 is stored as:
 * {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x10000000}
 *
 * All functions that output to an FF_NUM are designed such that statements
 * like "ff_add (&a, &a, &a)" are valid.
 */
#ifndef __FINITE_FIELD_H__
#define __FINITE_FIELD_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint32_t z[8];
} FF_NUM;


/* out = a */
void ff_copy (FF_NUM *const out, FF_NUM const *const a);

/* out = 0 */
void ff_zero (FF_NUM *const out);

/* a == 0 */
bool ff_is_zero (FF_NUM const *const a);

/* a == 1 */
bool ff_is_one (FF_NUM const *const a);

/* Returns 1 if a > b
 * Returns -1 if a < b
 * Returns 0 if a == b */
int ff_compare (FF_NUM const *const a, FF_NUM const *const b);

/* out = a + b (mod p) */
void ff_add (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b, FF_NUM const *const p);

/* out = a - b (mod p) */
void ff_sub (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b, FF_NUM const *const p);

/* out = a * b (mod p) */
void ff_mul (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const b, FF_NUM const *const p);

/* out = a ^ -1 (mod p)
 * NOTE: p must be prime! */
void ff_inv (FF_NUM *const out, FF_NUM const *const a, FF_NUM const *const p);

/* out = a >> 1 */
void ff_rshift1 (FF_NUM *const out, FF_NUM const *const a);

/* (q, rem) = a / 58 */
void ff_div58 (FF_NUM *const out_q, FF_NUM *const out_rem, FF_NUM const *const a);

/* Return log2(a) */
uint32_t ff_num_bits (FF_NUM const *const a);

/* Return a random number in the range [1, n - 1]
 * NOTE: If n is 0, the range will be [0, 2^256-1] */
void ff_rand (FF_NUM *const out, FF_NUM const *const n);

/* Copy a into the 8-bit array out, MSB first.
 * out must have at least 32 bytes available.
 * Returns the number of bytes written (32). */
uint32_t ff_serialize (uint8_t *out, FF_NUM const *const a);

/* Copy a into (out), Big Endian.
 * (a) must be at least 32 bytes.
 */
void ff_deserialize (FF_NUM *const out, uint8_t const a[static 32]);

/* Write the DER encoding of (a) to (out).
 * Will not write more than (maxlen) bytes.
 * Returns number of bytes written, or 0 if there wasn't enough space.
 */
uint32_t ff_der_serialize (uint8_t *const out, uint32_t maxlen, FF_NUM const *const a);

#endif
