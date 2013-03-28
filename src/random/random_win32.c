#include <random.h>
#include "util.h"

// TODO: Everything

void random_init (void)
{
}


uint32_t random_uint32 (void)
{
	error_state ();
	return 0;
}


/* TODO: It may be useful to feed into a DRBG, HMAC, or hash to hide the
 * state of the TRNG. Though it probably isn't necessary, because this is a
 * TRNG, it's a good defense in the worst case scenarios.
 */
void random_bytes (uint8_t *dst, uint32_t len)
{
	error_state ();
}
