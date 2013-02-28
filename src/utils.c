#include <strong-arm/utils.h>


/* This is needed until C11's memset_c becomes more prevalent. */
void secure_memset (void *b, int c, size_t len)
{
	if (b == NULL) return;

	volatile unsigned char *p = b;

	while (len--)
	{
		*p++ = c;
	}
}
