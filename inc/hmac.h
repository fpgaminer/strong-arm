/*
 * HMAC (Hash-based Message Authentication Code) Calculation based on SHA-256.
 */

#ifndef __HMAC_H__
#define __HMAC_H__

#include <stdint.h>


/* Write the HMAC hash of (msg) of length (mlen) with key (key) of length (klen) to (hash).
 * (hash) should be at least 32 bytes. */
void HMAC (uint8_t *const hash, uint8_t const *const key, uint32_t klen, uint8_t const *const msg, uint32_t mlen);

#endif
