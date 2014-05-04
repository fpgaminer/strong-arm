#include <stdio.h>
#include <string.h>
#include <minunit.h>
#include <strong-arm/ecdsa.h>


void print_ff (FF_NUM const *const a)
{
	for (int i = 7; i >= 0; --i)
		printf ("%08X", a->z[i]);
}


START_TEST (test_mul)
{
	const FF_NUM m1 = {0x3BF99522,0x8EE3C261,0xFA157859,0x624D4CC0,0x290311A3,0xA65527F7,0xA97A2479,0xAA5E28D6};
	const FF_NUM X1 = {0x39FF49C6,0xCFACDFA2,0x9FD54EBC,0x099AA0AD,0x5133A6BA,0x3D192B3C,0x0E4F0839,0x34F9460F};
	const FF_NUM Y1 = {0x0F2CA232,0x01BB419E,0x686CB5A9,0x7728A960,0x7A91E7DD,0x23F6D25A,0xD730FD89,0x0B71EA9B};
	const FF_NUM m2 = {0x4DEECCEC,0x7E1E181E,0x4C42422A,0x44B8A052,0x4AD44C40,0xEFFF13EE,0xDCD12581,0x376A3A2C};
	const FF_NUM X2 = {0x987DABA1,0x415EE4FE,0x3AA60A3E,0xFED471EF,0x81372CA6,0x92E5B36C,0xFCD4B0BD,0x14890E61};
	const FF_NUM Y2 = {0x2263F982,0xE66ADC27,0xDBE23397,0xD1C2B7B0,0xEE0EB6DC,0x2D3BCA67,0x9F752AB4,0x297B858D};
	const FF_NUM m3 = {0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};
	const FF_NUM m4 = {0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};
	
	EC_POINT Q;

	ec_mul (&Q, &m1, &ec_G);
	mu_assert (ff_compare (&(Q.x), &X1) == 0, "ec_mul should multiply G by a scalar value.");
	mu_assert (ff_compare (&(Q.y), &Y1) == 0, "ec_mul should multiply G by a scalar value.");

	ec_mul (&Q, &m2, &ec_G);
	mu_assert (ff_compare (&(Q.x), &X2) == 0, "ec_mul should multiply G by a scalar value.");
	mu_assert (ff_compare (&(Q.y), &Y2) == 0, "ec_mul should multiply G by a scalar value.");

	ec_mul (&Q, &m3, &ec_G);
	mu_assert (ff_compare (&(Q.x), &(ec_G.x)) == 0, "ec_mul should return G when multiplying G by 1.");
	mu_assert (ff_compare (&(Q.y), &(ec_G.y)) == 0, "ec_mul should return G when multiplying G by 1.");

	ec_mul (&Q, &m4, &ec_G);
	mu_assert (ff_is_zero (&(Q.x)), "ec_mul should return Infinity when multiply G by n.");
	mu_assert (ff_is_zero (&(Q.x)), "ec_mul should return Infinity when multiply G by n.");

	ec_mul (&Q, &ec_n, &ec_G);
	mu_assert (ff_is_zero (&(Q.x)), "ec_mul should return Infinity when multiplying G by n.");
	mu_assert (ff_is_zero (&(Q.y)), "ec_mul should return Infinity when multiplying G by n.");
}
END_TEST


/* Random unit test for ec_mul and ec_add */
START_TEST (test_random_addmul)
{
	FF_NUM a, b, c;
	EC_POINT P, Q, R, T1, T2;

	for (int passes = 0; passes < 10; ++passes)
	{
		ff_rand (&a, &ec_n);
		ff_rand (&b, &ec_n);
		ff_add (&c, &a, &b, &ec_n);
		ec_mul (&P, &a, &ec_G);
		ec_mul (&Q, &b, &ec_G);
		ec_mul (&R, &c, &ec_G);
		ec_add (&T1, &P, &Q);
		ec_add (&T2, &Q, &P);

		if (ff_compare (&(T1.x), &(R.x)) != 0 || ff_compare (&(T1.y), &(R.y)) != 0 || ff_compare (&(T2.x), &(R.x)) != 0 || ff_compare (&(T2.y), &(R.y)) != 0)
		{
			printf ("a: ");
			print_ff (&a);
			printf ("\nb: ");
			print_ff (&b);
			printf ("\nc: ");
			print_ff (&c);
			printf ("\nP: 04");
			print_ff (&(P.x));
			print_ff (&(P.y));
			printf ("\nQ: 04");
			print_ff (&(Q.x));
			print_ff (&(Q.y));
			printf ("\nR: 04");
			print_ff (&(R.x));
			print_ff (&(R.y));
			printf ("\nT1: 04");
			print_ff (&(T1.x));
			print_ff (&(T1.y));
			printf ("\nT2: 04");
			print_ff (&(T2.x));
			print_ff (&(T2.y));
			printf ("\n");
			return "If c = a + b is true then the following should be true: aG + bG == cG and bG + aG == cG.";
		}
	}
}
END_TEST


/* A real signature and public key pulled from the Bitcoin blockchain. */
START_TEST (test_verify)
{
	/* TXN: 409ed7df265df4c64457b0f99af11b18a35b450fa585eddd12436fd2cd6e2170 */
	const EC_POINT pubkey = {
		{{0xad3268c7,0x66e9e496,0x9c52baa0,0x337fc245,0xf29d7977,0x2c633098,0x6a882ddb,0xed541a9a}},
		{{0x3d4f2412,0x1304ce26,0xda1fc20f,0xf127283a,0xeeaabe87,0x05226d42,0x7437ccf7,0x4786717f}}
	};
	const FF_NUM r = {{0xB6A4DF24,0x95E23954,0xD7ACE345,0x1C9AD8EC,0x182A6D4A,0x93D2E9DB,0xED96E0A8,0x5C9521E0}};
	const FF_NUM s = {{0x2FA43157,0xAD57CD5C,0x8769FEB0,0x788A8A34,0x39DC5910,0x5C62F71D,0xEFFBAEAF,0xFFF839DE}};
	const uint8_t hash_str[] = {0x3d,0x66,0xae,0x70,0x09,0x4f,0x28,0xe4,0xc4,0xa2,0xc3,0xf2,0x63,0x9b,0xb5,0xf6,0x4e,0x0f,0xfb,0x1e,0x76,0x6f,0x12,0x1f,0x08,0x7b,0xac,0x81,0x92,0x52,0xd8,0x62};
	FF_NUM hash = {0};
	const FF_NUM bad_hash = {{0xDEADBEEF,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000}};
	const EC_POINT badpubkey = {{{0}},{{0}}};
	const EC_POINT badpubkey2 = {{{1}},{{2}}};
	const EC_POINT badpubkey3 = {
		{{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF}},
		{{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF}}
	};

	ff_deserialize (&hash, hash_str);
	
	mu_assert (ec_verify (&hash, &pubkey, &r, &s), "ec_verify should return true on a valid signature.");
	mu_assert (!ec_verify (&bad_hash, &pubkey, &r, &s), "ec_verify should return false on an invalid signature.");
	mu_assert (!ec_verify (&hash, &badpubkey, &r, &s), "ec_verify should return false on an invalid signature.");
	mu_assert (!ec_verify (&hash, &badpubkey2, &r, &s), "ec_verify should return false on an invalid signature.");
	mu_assert (!ec_verify (&hash, &badpubkey3, &r, &s), "ec_verify should return false on an invalid signature.");
}
END_TEST


/* Test corner cases */
START_TEST (test_sign_corner)
{
	EC_POINT pubkey;
	FF_NUM privkey = {0x00000001,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000};
	const FF_NUM hash = {0};
	FF_NUM r, s;
	const FF_NUM hash2 = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};

	ec_mul (&pubkey, &privkey, &ec_G);
	ec_sign (&r, &s, &hash, &privkey);
	mu_assert (ec_verify (&hash, &pubkey, &r, &s), "ec_sign should create a signature that can be verified with ec_verify when the private key is 1.");

	ec_create_key (&privkey, &pubkey);
	ec_sign (&r, &s, &hash, &privkey);
	mu_assert (ec_verify (&hash, &pubkey, &r, &s), "ec_sign should create a signature that can be verified with ec_verify when the hash is 0.");

	ec_sign (&r, &s, &hash2, &privkey);
	mu_assert (ec_verify (&hash2, &pubkey, &r, &s), "ec_sign should create a signature that can be verified with ec_verify when the hash is 2^256-1.");
}
END_TEST


/* Create random key pairs, sign random messages, and verify the signatures. */
START_TEST (test_random_sign)
{
	EC_POINT pubkey;
	FF_NUM privkey, hash, r, s;
	const FF_NUM max_hash = {0};	// Any 256-bit number
	
	for (int passes = 0; passes < 10; ++passes)
	{
		ec_create_key (&privkey, &pubkey);
		ff_rand (&hash, &max_hash);
		ec_sign (&r, &s, &hash, &privkey);
	
		if (!ec_verify (&hash, &pubkey, &r, &s))
		{
			printf ("ec_sign failed to create a signature that could be verified with ec_verify.");
			printf ("Private Key: ");
			print_ff (&privkey);
			printf ("\nPublic Key: 04");
			print_ff (&(pubkey.x));
			print_ff (&(pubkey.y));
			printf ("\nHash: ");
			print_ff (&hash);
			printf ("\nr: ");
			print_ff (&r);
			printf ("\ns: ");
			print_ff (&s);
			printf ("\n");
			return "ec_sign should create a signature that can be verified with ec_verify for all possible hash values.";
		}
	}
}
END_TEST


char *test_ecdsa (void)
{
	mu_run_test (test_mul);
	mu_run_test (test_random_addmul);
	mu_run_test (test_verify);
	mu_run_test (test_sign_corner);
	mu_run_test (test_random_sign);
	
	return 0;
}
