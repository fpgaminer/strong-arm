#ifndef __AES_H__
#define __AES_H__

#include <stdint.h>


/* Encrypt 128-bits of plaintext using the 256-bit key.
 * The 128-bit result is written to ciphertext.
 *
 * It is safe for ciphertext, key, and plaintext to point to the same data.
 *
 * Obviously, only use this with a standard block mode (CTR is the author's recommendation).
 */
void aes256_encrypt_block (uint8_t ciphertext[static 16], uint8_t const key[static 32], uint8_t const plaintext[static 16]);

#endif
