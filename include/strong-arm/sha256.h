#ifndef __SHA256_H__
#define __SHA256_H__

#include <stdint.h>
#include <stdbool.h>


typedef struct {
	uint32_t state[8];
	uint32_t chunk[16];
	uint32_t chunk_len;
	uint32_t totallen;
} SHA256_STATE;


/* Write the SHA256 256-bit hash into hash, for the given msg of length len. */
void SHA256 (uint8_t *hash, uint8_t const * msg, uint32_t len);


/* Call this method repeatedly with chunks of data to calculate a SHA256 hash of
 * all the data. The first call must set first to true. The last call must set 
 * last to true.
 * The 256-bit result is not written to hash until the last call.
 * It is valid to set both first and last to true.
 */
void SHA256_partial (uint8_t *hash, SHA256_STATE *const state, uint8_t const *src, uint32_t len, bool const first, bool const last);

#endif
