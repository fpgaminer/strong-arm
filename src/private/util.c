#include "util.h"

#ifdef TARGET_STM32F4
	#include <stm32f4xx.h>
#endif


// TODO: Go into low power mode, blink LED? Maybe even poweroff?
void error_state (void)
{
	// Disable interrupts
#ifdef TARGET_STM32F4
	__disable_irq ();
#endif

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
