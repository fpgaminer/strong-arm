#ifndef __ASN1_H__
#define __ASN1_H__

#include <strong-arm/finite_field.h>


/* DER encode an ECDSA signature.
 * Returns the number of bytes written, or 0 on error.
 * Will not write more than max_len bytes, and returns 0 if it would.
 */
uint32_t der_encode_signature (uint8_t *const dst, uint32_t max_len, FF_NUM const *const r, FF_NUM const *const s);

#endif
