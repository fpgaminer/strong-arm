#include <stdio.h>
#include <stdarg.h>
#include "minunit.h"
#include <strong-arm.h>

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


/* All of this dbg_write stuff is a hack to get some kind of printf working.
 * I couldn't get semihosting to work with openocd/gdb yet.
 * It still doesn't work yet, so we just throw a breakpoint and call it a day.
 */
#define TARGET_REQ_DEBUGMSG_ASCII			0x01
#define NVIC_DBG_DATA_R         (*((volatile unsigned short *)0xE000EDF8))
#define BUSY    1


void dbg_write(unsigned long dcc_data)
{
	int len = 4;

	while (len--)
	{
		/* wait for data ready */
		while (NVIC_DBG_DATA_R & BUSY);

		/* write our data and set write flag - tell host there is data*/
		NVIC_DBG_DATA_R = (unsigned short)(((dcc_data & 0xff) << 8) | BUSY);
		dcc_data >>= 8;
	}
}


void dbg_write_str(const char *msg)
{
	__asm__ ("BKPT");

	/*long len;
	unsigned long dcc_data;

	for (len = 0; msg[len] && (len < 65536); len++);

	dbg_write(TARGET_REQ_DEBUGMSG_ASCII | ((len & 0xffff) << 16));

	while (len > 0)
	{
		dcc_data = msg[0]
			| ((len > 1) ? msg[1] << 8 : 0x00)
			| ((len > 2) ? msg[2] << 16 : 0x00)
			| ((len > 3) ? msg[3] << 24 : 0x00);
		dbg_write(dcc_data);

		msg += 4;
		len -= 4;
	}*/
}


void my_printf (const char *format, ...)
{
	va_list arg;
	char buffer[1024] = {0};

	va_start (arg, format);
	snprintf (buffer, 1023, format, arg);
	va_end (arg);

	__asm__ ("BKPT");

	dbg_write_str (buffer);
}




static char *all_tests ()
{
	char *msg;

	/*
	if ((msg = test_finite_field ())) return msg;
	//if (msg = test_ecdsa ()) return msg;	// TODO: Re-enable
	if ((msg = test_ripemd160 ())) return msg;
	if (msg = test_sha256 ()) return msg;
	if (msg = test_base58 ()) return msg;
	//if (msg = test_random ()) return msg;	// TODO: Re-enable
	if (msg = test_hmac ()) return msg;
	if (msg = test_pbkdf2()) return msg;
	//if (msg = test_aes ()) return msg;	// TODO
	if (msg = test_drbg ()) return msg;*/
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
