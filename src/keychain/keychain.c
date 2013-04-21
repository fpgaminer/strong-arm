#include <strong-arm/keychain.h>
#include <strong-arm/aes.h>
#include <strong-arm/random.h>
#include <strong-arm/utils.h>
#include "util.h"


const FF_NUM restricted_ec_n = {{0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6, 0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};


void keychain_create (FF_NUM *const out_base_private_key, uint8_t out_chain_key[static 32])
{
	/* Random private key */
	ff_rand (out_base_private_key, &restricted_ec_n);

	/* Random chain key */
	random_bytes (out_chain_key, 32);
}


void keychain_calculate (FF_NUM *const out_privkey, EC_POINT *const out_pubkey, FF_NUM const *const base_private_key, uint8_t const chain_key[static 32], uint8_t const index[static 16])
{
	uint8_t encrypted_index[32] = {0};
	FF_NUM private_key_offset = {0};
	FF_NUM private_key = {0};

	if (ff_compare (base_private_key, &restricted_ec_n) >= 0 || ff_is_zero (base_private_key))
		error_state ();

	/* Calculate H(k,i) */
	aes256_encrypt_block (encrypted_index+16, chain_key, index);

	/* Convert into FF_NUM */
	/* Highest 128-bits are left at 0 */
	ff_deserialize (&private_key_offset, encrypted_index);

	/* Add to Base Private Key */
	ff_add (&private_key, base_private_key, &private_key_offset, &ec_n);

	/* Return Results */
	if (out_privkey)
		ff_copy (out_privkey, &private_key);

	if (out_pubkey)
		ec_mul (out_pubkey, &private_key, &ec_G);

	secure_memset (encrypted_index, 0, sizeof (encrypted_index));
	secure_memset (&private_key_offset, 0, sizeof (private_key_offset));
	secure_memset (&private_key, 0, sizeof (private_key));
}
