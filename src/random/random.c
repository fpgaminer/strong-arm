#include <stm32f4xx.h>
#include <random.h>
#include "util.h"

#define RCC_AHB2Periph_RNG               ((uint32_t)0x00000040)
#define RNG_FLAG_DRDY               ((uint8_t)0x0001) /*!< Data ready */
#define RNG_FLAG_CECS               ((uint8_t)0x0002) /*!< Clock error current status */
#define RNG_FLAG_SECS               ((uint8_t)0x0004) /*!< Seed error current status */


static __IO uint32_t prev_rng;


static uint32_t read_random_number (void)
{
	while (1)
	{
		if (RNG->SR & RNG_FLAG_CECS)
			error_state ();
		if (RNG->SR & RNG_FLAG_SECS)
			error_state ();
		
		if ((RNG->SR & RNG_FLAG_DRDY) == 0)
			continue;
		
		return RNG->DR;
	}
}

void random_init (void)
{
	RCC->AHB2ENR |= RCC_AHB2Periph_RNG;	// Enable Clock
	RCC->AHB2RSTR |= RCC_AHB2Periph_RNG;	// Enable Reset of RNG
	RCC->AHB2RSTR &= ~RCC_AHB2Periph_RNG;	// Disable Reset of RNG
	volatile uint32_t tmp = RNG->DR;		// Clear the RNG data register
	RNG->CR |= RNG_CR_RNGEN;		// Enable RNG
	
	// Discard first result
	// ref: FIPS PUB 140-2, 4.9.2
	prev_rng = read_random_number ();
}

// TODO: Find a way to prevent "stale" data, or prove that there is no such thing as stale data.
uint32_t random_uint32 (void)
{
	uint32_t tmp = read_random_number ();
	
	// Continuous random number generator test
	// ref: FIPS PUB 140-2, 4.9.2
	if (tmp == prev_rng)
		error_state ();
	prev_rng = tmp;
	
	return tmp;
}
