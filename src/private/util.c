#include "util.h"

// TODO: Disable interrupts, go into low power mode. Maybe even poweroff?
void error_state (void)
{
	while (1)
	{
	}
}

uint32_t blog2 (uint32_t a)
{
	const uint32_t b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
	const uint32_t S[] = {1, 2, 4, 8, 16};
	uint32_t r = 0;

	for (int i = 4; i >= 0; --i)
	{
		if (a & b[i])
		{
			a >>= S[i];
			r |= S[i];
		} 
	}

	return r;
}

