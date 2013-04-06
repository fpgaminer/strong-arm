#include <stdlib.h>
#include <random.h>
#include <time.h>
#include "util.h"

// TODO: Everything
// TODO: This is just for testing at the moment and is not cryptographically strong. DO NOT USE!!
// TODO: Probably replace with HMAC_DRBG and a good entropy source.

void random_init (void)
{
	srand (time (NULL));
}


uint32_t random_uint32 (void)
{
	return rand ();
	//error_state ();
	//return 0;
}


/* TODO: It may be useful to feed into a DRBG, HMAC, or hash to hide the
 * state of the TRNG. Though it probably isn't necessary, because this is a
 * TRNG, it's a good defense in the worst case scenarios.
 */
void random_bytes (uint8_t *dst, uint32_t len)
{
	while (len)
	{
		*dst = random_uint32 () & 0xFF;
		--len;
	}
}
