#include <stdio.h>
#include <minunit.h>
#include <ecdsa.h>

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
	
	EC_POINT Q;

	ec_mul (&Q, &m1, &ec_G);
	mu_assert (ff_compare (&(Q.x), &X1) == 0, "ec_mul should multiply G by a scalar value.");
	mu_assert (ff_compare (&(Q.y), &Y1) == 0, "ec_mul should multiply G by a scalar value.");

	ec_mul (&Q, &m2, &ec_G);
	mu_assert (ff_compare (&(Q.x), &X2) == 0, "ec_mul should multiply G by a scalar value.");
	mu_assert (ff_compare (&(Q.y), &Y2) == 0, "ec_mul should multiply G by a scalar value.");

	ec_mul (&Q, &ec_n, &ec_G);
	mu_assert (ff_is_zero (&(Q.x)), "ec_mul should return Infinity when multiplying G by n");
	mu_assert (ff_is_zero (&(Q.y)), "ec_mul should return Infinity when multiplying G by n");
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
/* TODO: Need to grab one. A bit difficult, because I need to calculate the hash by hand ... */
START_TEST (test_sign)
{
	const EC_POINT pubkey = {
		{{0}},
		{{0}}
	};
	const FF_NUM r = {0};
	const FF_NUM s = {0};
	const FF_NUM hash = {0};
	const FF_NUM bad_hash = {0};
	const EC_POINT badpubkey = {{{0}},{{0}}};
	const EC_POINT badpubkey2 = {{{1}},{{2}}};
	const EC_POINT badpubkey3 = {
		{{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF}},
		{{0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF}}
	};
	
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
	mu_run_test (test_sign_corner);
	mu_run_test (test_random_sign);
	
	return 0;
}
