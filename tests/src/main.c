#include <stdio.h>
#include <stdint.h>
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
char *test_keychain (void);
char *test_threefish (void);


// Manually implemented semihosting, because I could not get GCC ARM Embedded's semihosting working.
// The semihosting protocol is documented in ARM DUI 0203J.
// This function calls SYS_WRITE (0x05).
#ifdef __arm__
int _write (int fd, char *ptr, int len)
{
	uint32_t data[3] = {(uint32_t)fd, (uint32_t)ptr, (uint32_t)len};

	__asm__ ("mov r1,%0; mov r0,$5; bkpt 0xab" :
	                                           : "r" (data)
	                                           : "r0", "r1"
	);

	return len;
}
#endif


static char *all_tests ()
{
	char *msg;


	if ((msg = test_finite_field ())) return msg;
	if ((msg = test_ecdsa ())) return msg;
	if ((msg = test_keychain ())) return msg;
	if ((msg = test_ripemd160 ())) return msg;
	if ((msg = test_sha256 ())) return msg;
	if ((msg = test_base58 ())) return msg;
	if ((msg = test_random ())) return msg;
	if ((msg = test_hmac ())) return msg;
	if ((msg = test_pbkdf2())) return msg;
	if ((msg = test_drbg ())) return msg;
	if ((msg = test_aes ())) return msg;
	if ((msg = test_threefish ())) return msg;
	
	return 0;
}


int main (void)
{
	strongarm_init ();
	
	char *result = all_tests ();
	
	if (result != 0)
		printf ("%s\n", result);
	else
		printf ("ALL TESTS PASSED\n");
	printf ("Tests run: %d\n", tests_run);
	
	return result != 0;
}
