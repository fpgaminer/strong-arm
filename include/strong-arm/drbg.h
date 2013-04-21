/*
 * Deterministic Random Bit Generators
 * There are Cryptographically Secure Pseudorandom Number Generators.
 *
 * See: NIST SP 800-90A
 *
 * Currently, only HMAC_DRBG is implemented, which is based on HMAC-SHA256.
 * HMAC_DRBG uses repetitions of HMAC to generate the pseudorandom sequence
 * from a given seed.
 *
 * security_strength: 256-bits (Page 64 of SP 800-57 Part 1, May 2011).
 *
 */
#ifndef __DRBG_H__
#define __DRBG_H__

#include <stdint.h>

typedef struct {
	uint8_t initialized;
	uint8_t K[32], V[32];
} DRBG_STATE;


/* Initialize the DRBG with the given 96-byte (seed). */
void HMAC_DRBG_Init (DRBG_STATE *const state, uint8_t const *const seed);

/* Generate 32 bytes of pseudorandom data.
 * (state) is updated during each call.
 *
 * WARNING: Do not call more than 2^48 times with a given seed!
 * WARNING: state _must_ have been initialized!
 */
void HMAC_DRBG_Generate (uint8_t *const dst, DRBG_STATE *const state);

#endif
