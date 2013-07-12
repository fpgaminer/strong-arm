#ifndef __AES_H__
#define __AES_H__

#include <stdint.h>


/* Encrypt 128-bits of plaintext using the 256-bit key.
 * The 128-bit result is written to ciphertext.
 *
 * It is safe for ciphertext, key, and plaintext to point to the same data.
 *
 * Obviously, only use this with a standard block mode.
 */
void aes256_encrypt_block (uint8_t ciphertext[static 16], uint8_t const key[static 32], uint8_t const plaintext[static 16]);


/* Encrypt or Decrypt plaintext of length len bytes using the given 256-bit key and 128-bit IV.
 * The result is written to ciphertext. ciphertext must be at least len bytes.
 *
 * This uses the CTR block mode.
 *
 * It is safe for ciphertext, key, iv, and plaintext to point to the same data.
 *
 * IV should always be unique. Never re-use the same IV!
 */
void aes256_crypt_ctr (uint8_t *ciphertext, uint8_t const key[static 32], uint8_t const iv[static 16], uint8_t const *plaintext, uint32_t len);

#endif
