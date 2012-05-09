#include <stdio.h>
#include <minunit.h>
#include <ecdsa.h>

void print_ff (FF_NUM const *const a)
{
	for (int i = 7; i >= 0; --i)
		printf ("%08X", a->z[i]);
}

/* Create random key pairs, sign random messages, and verify the signatures. */
START_TEST (test_random_sign)
{
	EC_POINT pubkey;
	FF_NUM privkey, hash, r, s;
	FF_NUM max_hash = {0};	// Any 256-bit number
	
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
	mu_run_test (test_random_sign);
	
	return 0;
}
