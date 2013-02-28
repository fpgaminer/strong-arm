/*
 * PBKDF2 Key Stretching.
 * Uses HMAC-SHA256 as the PRF.
*/

#ifndef __PBKDF2_H__
#define __PBKDF2_H__

#include <stdint.h>


/* Write a derived key to (derived_key), of length (keylen) bytes.
 * The derived key is generated from (password) and (salt).
 * This is a One-Way Function. (password) and (salt) cannot be easily guessed from (derived_key).
 * (iterations) determines how difficult it will be to brute-force (password).
 * Between 1,000 and 10,000 iterations is currently recommended (As of 2012).
 * 
 * WARNING: (salt) must not exceed 16 bytes!
 * WARNING: The memory for (dervied_key) should not overlap password!
 */
void PBKDF2 (uint8_t *const derived_key, uint8_t const *const password, uint32_t passlen, uint8_t const *const salt, uint32_t saltlen, uint32_t iterations, uint32_t keylen);

#endif
