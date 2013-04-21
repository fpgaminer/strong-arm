/* 
 * Implements Key Chain Functionality using ECDSA.
 *
 * A Key Chain consists of a Base Key Pair, a Chain Key, and a sequence of Key
 * Pairs derived from them.
 *
 * d_b = Base Private Key = random integer in range [1, ec_n - 2**128)
 * Q_b = Base Public Key = d_b * G
 * k = Chain Key = random integer in range [0, 2**256)
 *
 * For key 'n', where n is a 128-bit integer
 * offset = AES256(k,n)
 * d_n = d_b + offset
 * Q_n = Q_b + offset * G
 *
 *
 * Note that the base private key is in a more restricted range, so that any
 * derived private key never equals 0 (which is invalid).
 *
 *
 * Designed by fpgaminer (fpgaminer@bitcoin-mining.com).
 *
 */

#ifndef __KEY_CHAIN_H__
#define __KEY_CHAIN_H__

#include <stdint.h>
#include <strong-arm/finite_field.h>
#include <strong-arm/ecdsa.h>


/* Generate a random key chain */
void key_chain_create (FF_NUM *const out_base_private_key, uint8_t out_chain_key[static 32]);

/* Calculate a derived key.
 * out_privkey and out_pubkey may either or both be NULL.
 */
void key_chain_calculate (FF_NUM *const out_privkey, EC_POINT *const out_pubkey, FF_NUM const *const base_private_key, uint8_t const chain_key[static 32], uint8_t const index[static 16]);

#endif
