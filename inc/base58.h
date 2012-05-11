/*
 * Base58 Encoding
 */

#ifndef __BASE58_H__
#define __BASE58_H__

#include <stdint.h>

/* Base58 encode src and write the result to dst.
 * src must be exactly 25 bytes.
 * dst must be at least 36 bytes, and will be NULL terminated.
 */
void base58_encode (uint8_t *const dst, uint8_t const *const src);

/* Base58Check encode src and write the result to dst.
 * src must be exactly 20 bytes.
 * dst must be at least 36 bytes, and will be NULL terminated.
 *
 * Base58Check prepends a version byte, and appends a 4 byte checksum before
 * Base58 encoding. The checksum is the first 4 bytes of
 * SHA256(SHA256(version + src))
 */
void base58check_encode (uint8_t *const dst, uint8_t version, uint8_t const *const src);

#endif
