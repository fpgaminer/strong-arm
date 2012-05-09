#include <stdio.h>
#include <minunit.h>

int tests_run = 0;

char *test_finite_field (void);


static char *all_tests ()
{
	char *msg;

	if (msg = test_finite_field ()) return msg;
	
	return 0;
}


int main (void)
{
	char *result = all_tests ();
	
	if (result != 0)
		printf ("%s\n", result);
	else
		printf ("ALL TESTS PASSED\n");
	printf ("Tests run: %d\n", tests_run);
	
	return result != 0;
}
