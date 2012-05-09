/* 
 * Implements Finite Field Arithmetic and Elliptic Curve Arithmetic
 * for calculating secp256k1 ECC.
 *
 * secp256k1 ECC
 * p = 2^256 - 2^32 - 2^9 - 2^8 - 2^7 - 2^6 - 2^4 - 1
 *
 */

#ifndef __ECDSA_H__
#define __ECDSA_H__

#include <stdint.h>
#include <finite_field.h>


/* Point on the secp256k1 elliptic curve. */
typedef struct {
	FF_NUM x, y;
} EC_POINT;


/* Constants */
extern const EC_POINT ec_G;
extern const FF_NUM ec_n;


/* out = A + B */
void ec_add (EC_POINT *const out, EC_POINT const *const A, EC_POINT const *const B);

/* out = A + A */
void ec_double (EC_POINT *const out, EC_POINT const *const P);

/* out = k * P */
void ec_mul (EC_POINT *const out, FF_NUM const *const k, EC_POINT const *const P);

/* Create a Random Private/Public Key Pair */
void ec_create_key (FF_NUM *const ret_privkey, EC_POINT *const ret_pubkey);

/* Create a signature given a 256-bit HASH z, and private key d. */
bool ec_sign (FF_NUM *const out_r, FF_NUM *const out_s, FF_NUM const *const z, FF_NUM const *const d);

/* Returns true if the given point is on the secp256k1 curve. */
bool ec_on_curve (EC_POINT const *const P);

/* Returns true if the given signature (r,s) is valid for the given public key Q
 * on the 256-bit HASH z. */
bool ec_verify (FF_NUM const *const z, EC_POINT const *const Q, FF_NUM const *const r, FF_NUM const *const s);

/* Serialize point P to out and return the number of bytes written.
 * Compressed points are 33 bytes, uncompressed are 65 */
uint32_t ec_serialize (uint8_t *out, EC_POINT const *const P, bool compressed);

#endif
