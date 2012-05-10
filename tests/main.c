#include <stdio.h>
#include <minunit.h>
#include <random.h>

int tests_run = 0;

char *test_finite_field (void);
char *test_ecdsa (void);
char *test_ripemd160 (void);
char *test_sha256 (void);


static char *all_tests ()
{
	char *msg;

	if (msg = test_finite_field ()) return msg;
	if (msg = test_ecdsa ()) return msg;
	if (msg = test_ripemd160 ()) return msg;
	if (msg = test_sha256 ()) return msg;
	//if (msg = test_random ()) return msg;	// TODO
	//if (msg = test_aes ()) return msg;	// TODO
	//if (msg = test_base58 ()) return msg;	// TODO
	
	return 0;
}


int main (void)
{
	random_init ();
	
	char *result = all_tests ();
	
	if (result != 0)
		printf ("%s\n", result);
	else
		printf ("ALL TESTS PASSED\n");
	printf ("Tests run: %d\n", tests_run);
	
	return result != 0;
}
