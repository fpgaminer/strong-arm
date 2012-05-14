#include <string.h>
#include <drbg.h>
#include <util.h>
#include <hmac.h>


/* Update (state) using 96-bytes of (provided_data). */
static void HMAC_DRBG_Update (DRBG_STATE *const state, uint8_t const *const provided_data);


static void HMAC_DRBG_Update (DRBG_STATE *const state, uint8_t const *const provided_data)
{
	uint8_t temp[129];

	memmove (temp, state->V, 32);
	temp[32] = 0x00;

	if (provided_data)
		memmove (temp + 33, provided_data, 96);

	/* K = HMAC (K, V || 0x00 || provided_data) */
	HMAC (state->K, state->K, 32, temp, provided_data ? 129 : 33);
	/* V = HMAC (K, V) */
	HMAC (state->V, state->K, 32, state->V, 32);

	if (!provided_data)
		return;

	memmove (temp, state->V, 32);
	temp[32] = 0x01;

	/* K = HMAC (K, V || 0x01 || provided_data) */
	HMAC (state->K, state->K, 32, temp, 129);
	/* V = HMAC (K, V) */
	HMAC (state->V, state->K, 32, state->V, 32);
}


void HMAC_DRBG_Init (DRBG_STATE *const state, uint8_t const *const seed)
{
	memset (state->K, 0x00, 32);
	memset (state->V, 0x01, 32);

	HMAC_DRBG_Update (state, seed);
	state->initialized = 0x30;
}


void HMAC_DRBG_Generate (uint8_t *const dst, DRBG_STATE *const state)
{
	if (state->initialized != 0x30)
		error_state ();

	/* V = HMAC (K, V) */
	HMAC (state->V, state->K, 32, state->V, 32);
	memmove (dst, state->V, 32);

	HMAC_DRBG_Update (state, 0);
}
