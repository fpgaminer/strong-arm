#ifndef __RIPEMD160_H__
#define __RIPEMD160_H__

#include <stdint.h>

/* Calculate the 160-bit RIPEMD160 hash of msg with length len and write the 
 * result to hash. */
void RIPEMD160 (uint8_t *hash, uint8_t const * msg, uint32_t len);

#endif
