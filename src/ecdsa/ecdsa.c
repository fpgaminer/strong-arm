#include <strong-arm/ecdsa.h>


const EC_POINT ec_G = {
	{{0x16F81798, 0x59F2815B, 0x2DCE28D9, 0x029BFCDB, 0xCE870B07, 0x55A06295, 0xF9DCBBAC, 0x79BE667E}},
	{{0XFB10D4B8, 0X9C47D08F, 0XA6855419, 0XFD17B448, 0X0E1108A8, 0X5DA4FBFC, 0X26A3C465, 0X483ADA77}}
};

const FF_NUM ec_n = {{0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};

const FF_NUM ec_p = {{0xFFFFFC2F, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};

const FF_NUM ec_b = {{0x00000007, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}};




void ec_add (EC_POINT *const out, EC_POINT const *const A, EC_POINT const *const B)
{
	FF_NUM slope, tmp, tmp1;
	FF_NUM x3, y3;
	
	if ((ff_is_zero (&(A->x))) && (ff_is_zero (&(A->y))))
	{
		ff_copy (&(out->x), &(B->x));
		ff_copy (&(out->y), &(B->y));
		return;
	}
	
	if ((ff_is_zero (&(B->x))) && (ff_is_zero (&(B->y))))
	{
		ff_copy (&(out->x), &(A->x));
		ff_copy (&(out->y), &(A->y));
		return;
	}

	if (ff_compare (&(A->x), &(B->x)) == 0)
	{
		ff_add (&tmp, &(A->y), &(B->y), &ec_p);

		if (ff_is_zero (&tmp))
		{
			// Infinity
			ff_zero (&(out->x));
			ff_zero (&(out->y));
			return;
		}
		else
		{
			ec_double (out, out);
			return;
		}
	}

	// Slope
	ff_sub (&tmp, &(B->y), &(A->y), &ec_p);
	ff_sub (&tmp1, &(B->x), &(A->x), &ec_p);
	ff_inv (&tmp1, &tmp1, &ec_p);
	ff_mul (&slope, &tmp, &tmp1, &ec_p);		// slope = (y2 - y1) / (x2 - x1)

	// x
	ff_mul (&tmp, &slope, &slope, &ec_p);
	ff_sub (&x3, &tmp, &(A->x), &ec_p);
	ff_sub (&x3, &x3, &(B->x), &ec_p);	// out.x = slope^2 - x1 - x2

	// y
	ff_sub (&tmp, &(A->x), &x3, &ec_p);
	ff_mul (&tmp, &slope, &tmp, &ec_p);
	ff_sub (&y3, &tmp, &(A->y), &ec_p);	// out.y = slope * (x1 - out.x) - y1

	ff_copy (&(out->x), &x3);
	ff_copy (&(out->y), &y3);
}


void ec_double (EC_POINT *const out, EC_POINT const *const P)
{
	FF_NUM slope, tmp, tmp1;
	FF_NUM x3, y3;

	if ((ff_is_zero (&(P->x))) && (ff_is_zero (&(P->y))))
	{
		ff_zero (&(out->x));
		ff_zero (&(out->y));
		return;
	}

	ff_mul (&tmp, &(P->x), &(P->x), &ec_p);	// tmp = x^2
	ff_add (&tmp1, &tmp, &tmp, &ec_p);	// tmp1 = 2 * x^2
	ff_add (&tmp, &tmp1, &tmp, &ec_p);	// tmp = 3 * x^2
	//ff_add (&tmp, &tmp, a);		// tmp = 3x^2 + a	// a == 0 for our curve
	ff_add (&tmp1, &(P->y), &(P->y), &ec_p);	// tmp1 = 2 * y
	ff_inv (&tmp1, &tmp1, &ec_p);			// tmp1 = 1 / (2y)
	ff_mul (&slope, &tmp, &tmp1, &ec_p);	// slope = (3x^2 + a) / (2y)
	ff_mul (&tmp, &slope, &slope, &ec_p);	// tmp = slope^2
	ff_sub (&x3, &tmp, &(P->x), &ec_p);
	ff_sub (&x3, &x3, &(P->x), &ec_p);	// x3 = slope^2 - 2x

	ff_sub (&tmp, &(P->x), &x3, &ec_p);	// tmp = x - x3
	ff_mul (&y3, &slope, &tmp, &ec_p);
	ff_sub (&y3, &y3, &(P->y), &ec_p);	// y3 = slope * (x - x3) - y

	ff_copy (&(out->x), &x3);
	ff_copy (&(out->y), &y3);
}


void ec_mul (EC_POINT *const out, FF_NUM const *const k, EC_POINT const *const P)
{
	EC_POINT Q;

	ff_zero (&(Q.x));
	ff_zero (&(Q.y));

	for (uint32_t i = 255; true; --i)
	{
		ec_double (&Q, &Q);

		if ((k->z[i>>5] >> (i&31)) & 1)
			ec_add (&Q, &Q, P);
		
		if (i == 0)
			break;
	}
	
	ff_copy (&(out->x), &(Q.x));
	ff_copy (&(out->y), &(Q.y));
}


void ec_create_key (FF_NUM *const ret_privkey, EC_POINT *const ret_pubkey)
{
	ff_rand (ret_privkey, &ec_n);
	ec_mul (ret_pubkey, ret_privkey, &ec_G);
}


// Sign z, the 256-bit HASH of a message m, with d, the private key.
// Returns a pair (r,s), which is the signature.
bool ec_sign (FF_NUM *const out_r, FF_NUM *const out_s, FF_NUM const *const z, FF_NUM const *const d)
{
	FF_NUM k, r, s;
	EC_POINT kG;

	// Private key must be within range [1, n - 1]
	if ((ff_is_zero (d)) || (ff_compare (d, &ec_n) >= 0))
		return false;
	
	while (1)
	{
		ff_rand (&k, &ec_n);
		ec_mul (&kG, &k, &ec_G);
		ff_zero (&r);
		ff_add (&r, &r, &(kG.x), &ec_n);	// r = kG.x (mod n)
		
		if (ff_is_zero (&r))
			continue;
		
		ff_inv (&k, &k, &ec_n);
		ff_mul (&s, &r, d, &ec_n);
		ff_add (&s, z, &s, &ec_n);
		ff_mul (&s, &k, &s, &ec_n);
		
		if (ff_is_zero (&s))
			continue;
		
		break;
	}

	ff_copy (out_r, &r);
	ff_copy (out_s, &s);

	return true;
}

bool ec_on_curve (EC_POINT const *const P)
{
	// y^2 = x^3 + a * x + b (mod p)
	FF_NUM tmp, y2;

	if (ff_compare (&(P->x), &ec_p) >= 0)
		return false;

	if (ff_compare (&(P->y), &ec_p) >= 0)
		return false;

	ff_mul (&tmp, &(P->x), &(P->x), &ec_p);
	ff_mul (&tmp, &tmp, &(P->x), &ec_p);	// tmp = x^3
	ff_add (&tmp, &tmp, &ec_b, &ec_p);	// tmp = x^3 + b
	ff_mul (&y2, &(P->y), &(P->y), &ec_p);	// y2 = y^2
	
	if (ff_compare (&y2, &tmp) != 0)
		return false;

	return true;
}

bool ec_verify (FF_NUM const *const z, EC_POINT const *const Q, FF_NUM const *const r, FF_NUM const *const s)
{
	FF_NUM w, u1, u2, x1;
	EC_POINT tmp, tmp2;

	// Check that the public key Q is not Infinity
	if ((ff_is_zero (&(Q->x))) && (ff_is_zero (&(Q->y))))
		return false;

	// Check that the public key Q is on the curve
	if ((!ec_on_curve (Q)))
		return false;

	// Check that nQ == Infinity
	ec_mul (&tmp, &ec_n, Q);
	if ((!ff_is_zero (&(tmp.x))) || (!ff_is_zero (&(tmp.y))))
		return false;

	// Check that r and s are in the range [1, n - 1]
	if ((ff_is_zero (r)) || (ff_is_zero (s)))
		return false;

	if ((ff_compare (r, &ec_n) >= 0) || (ff_compare (s, &ec_n) >= 0))
		return false;

	// Verify the signature
	ff_inv (&w, s, &ec_n);	// w = s^-1
	ff_mul (&u1, z, &w, &ec_n);	// u1 = z * w
	ff_mul (&u2, r, &w, &ec_n);	// u2 = r * w
	ec_mul (&tmp, &u1, &ec_G);
	ec_mul (&tmp2, &u2, Q);
	ec_add (&tmp, &tmp, &tmp2);	// tmp = u1 * G + u2 * Q
	ff_zero (&x1);
	ff_add (&x1, &x1, &(tmp.x), &ec_n);	// x1 = tmp.x (mod n)

	if (ff_compare (&x1, r) != 0)
		return false;
	
	return true;
}

uint32_t ec_serialize (uint8_t *out, EC_POINT const *const P, bool compressed)
{
	if (!compressed) {
		if (out) {
			out[0] = 0x04;
			ff_serialize (out+1, &(P->x));
			ff_serialize (out+1+32, &(P->y));
		}
		return 65;
	}
	else {
		if (out) {
			out[0] = (P->y.z[0] & 1) ? 0x03 : 0x02;	// odd/even?
			ff_serialize (out+1, &(P->x));
		}
		return 33;
	}
}
