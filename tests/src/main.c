#include <stdio.h>
#include <stdarg.h>
#include "minunit.h"
#include <strong-arm/strong-arm.h>

int tests_run = 0;

char *test_finite_field (void);
char *test_ecdsa (void);
char *test_ripemd160 (void);
char *test_sha256 (void);
char *test_base58 (void);
char *test_random (void);
char *test_hmac (void);
char *test_pbkdf2 (void);
char *test_drbg (void);
char *test_aes (void);


void dbg_write_str(const char *msg)
{
#ifdef __arm__
	__asm__ ("BKPT");
#else
	printf ("%s", msg);
#endif
}


void my_printf (const char *format, ...)
{
	va_list arg;
	char buffer[1024] = {0};

	va_start (arg, format);
	vsnprintf (buffer, 1023, format, arg);
	va_end (arg);

	dbg_write_str (buffer);
}


static char *all_tests ()
{
	char *msg;


	if ((msg = test_finite_field ())) return msg;
	if ((msg = test_ecdsa ())) return msg;
	if ((msg = test_ripemd160 ())) return msg;
	if ((msg = test_sha256 ())) return msg;
	if ((msg = test_base58 ())) return msg;
	//if ((msg = test_random ())) return msg;
	if ((msg = test_hmac ())) return msg;
	if ((msg = test_pbkdf2())) return msg;
	if ((msg = test_drbg ())) return msg;
	if ((msg = test_aes ())) return msg;
	
	return 0;
}


int main (void)
{
	strongarm_init ();
	
	char *result = all_tests ();
	
	if (result != 0)
		my_printf ("%s\n", result);
	else
		my_printf ("ALL TESTS PASSED\n");
	my_printf ("Tests run: %d\n", tests_run);
	
	return result != 0;
}
