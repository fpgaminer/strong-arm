/*
 * HMAC (Hash-based Message Authentication Code) Calculation based on SHA-256.
 */

#ifndef __HMAC_H__
#define __HMAC_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <strong-arm/sha256.h>


typedef struct {
	SHA256_STATE sha256_state;
	uint8_t padded_key[64];
} HMAC_STATE;



/* Write the HMAC hash of (msg) of length (mlen) with key (key) of length (klen) to (hash).
 * (hash) should be at least 32 bytes. */
void HMAC (uint8_t *const hash, uint8_t const *const key, uint32_t klen, uint8_t const *const msg, uint32_t mlen);


/* Call this method repeatedly with chunks of data to calculate the HMAC hash of all the data.
 * The first call must set 'first' to true.  The last call must set 'last' to true.
 * The 256-bit result is not written to hash until the last call.
 * It is valid to set both first and last to true.
 * All of key must be provided on the first call, but not on any subsequent calls.
 */
void HMAC_partial (void *hash, HMAC_STATE *const state, void const *const key, size_t klen, void const *msg, size_t mlen, bool const first, bool const last);

#endif
