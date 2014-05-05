/*
 * Threefish Block Cipher, based on Skein V1.3.
 *
 * Currently only 512-bit mode is implemented.
 */
#ifndef __THREEFISH_H__
#define __THREEFISH_H__

#include <stdint.h>


/* Encrypt 512-bits of plaintext using the 512-bit key and 128-bit tweak.
 * The 512-bit result is written to ciphertext.
 *
 * It is safe for ciphertext, key, tweak, and plaintext to point to the same data.
 *
 * Obviously, only use this with a standard block mode.
 */
void threefish512_encrypt_block (uint8_t ciphertext[static 64], uint8_t const key[static 64], uint8_t const tweak[static 16], uint8_t const plaintext[static 64]);


/* Decrypt 512-bits of ciphertext using the 512-bit key and 128-bit tweak.
 * The 512-bit result is written to plaintext.
 *
 * It is safe for plaintext, key, tweak, and ciphertext to point to the same data.
 *
 * Obviously, only use this with a standard block mode.
 */
void threefish512_decrypt_block (uint8_t plaintext[static 64], uint8_t const key[static 64], uint8_t const tweak[static 16], uint8_t const ciphertext[static 64]);


#endif
