#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>

// These functions will enter an error state if any errors occur.
// This error state cannot be exitted from and the device will need to be
// power-cycled.
// ref: FIPS 140-2, 4.9 Self-Tests

/* TODO: Need to keep this private. Only strongarm_init is allowed to call this. */
// Initialize the random number module.
// Call once at the beginning of operation.
void random_init (void);

// Get an unsigned 32-bit random number.
// IMPORTANT: It is recommended that, whenever one or more results from this
// function are going to be used, the first is discarded. This prevents possible
// stale data.
uint32_t random_uint32 (void);


/* Fill (dst) with (len) random bytes. */
void random_bytes (uint8_t *dst, uint32_t len);

#endif
