#include <stm32f4xx.h>
#include <strong-arm/random.h>
#include "util.h"

#define RCC_AHB2Periph_RNG    ((uint32_t)0x00000040)
#define RNG_FLAG_DRDY         ((uint8_t)0x0001) /*!< Data ready */
#define RNG_FLAG_CECS         ((uint8_t)0x0002) /*!< Clock error current status */
#define RNG_FLAG_SECS         ((uint8_t)0x0004) /*!< Seed error current status */
#define RNG_IT_CEI            ((uint8_t)0x20)   /*!< Clock error interrupt */
#define RNG_IT_SEI            ((uint8_t)0x40)   /*!< Seed error interrupt */


static uint32_t read_random_number (void);


static volatile uint32_t prev_rng0 = 0, prev_rng1 = 0;


static uint32_t read_random_number (void)
{
	// Disable interrupts, since conditionally reading RNG->DR based on
	// RNG->SR isn't atomic.
	uint32_t primask = __get_PRIMASK ();
	__disable_irq ();

	while (1)
	{
		uint32_t status = RNG->SR;
		uint32_t result;

		if ((status & RNG_FLAG_CECS) || (status & RNG_IT_CEI))
			error_state ();
		if ((status & RNG_FLAG_SECS) || (status & RNG_IT_SEI))
			error_state ();
		
		if ((status & RNG_FLAG_DRDY) == 0)
			continue;
		
		result = RNG->DR;
		__set_PRIMASK (primask);    // Re-enable interrupts

		return result;
	}
}


void random_init (void)
{
	RCC->AHB2ENR |= RCC_AHB2Periph_RNG;     // Enable Clock
	RCC->AHB2RSTR |= RCC_AHB2Periph_RNG;    // Enable Reset of RNG
	RCC->AHB2RSTR &= ~RCC_AHB2Periph_RNG;   // Disable Reset of RNG
	volatile uint32_t tmp = RNG->DR;        // Clear the RNG data register
	(void)(tmp);
	RNG->CR = RNG_CR_RNGEN;                 // Enable RNG
	
	// Discard first 64-bits
	// ref: FIPS PUB 140-2, 4.9.2
	// NOTE: I have chosen to increase 'n' to 64 for the purposes of the 
	// continuous random number generator test. The test should still be
	// effective, and "false positives" will be less likely.
	prev_rng0 = read_random_number ();
	prev_rng1 = read_random_number ();
}


uint32_t random_uint32 (void)
{
	uint32_t tmp0 = read_random_number ();
	uint32_t tmp1 = read_random_number ();
	
	// Continuous random number generator test
	// ref: FIPS PUB 140-2, 4.9.2
	if (tmp0 == prev_rng0 && tmp1 == prev_rng1)
		error_state ();
	prev_rng0 = tmp0;
	prev_rng1 = tmp1;
	
	// We return the newest 32-bits, because tmp0 may have been sitting in
	// the RNG's register for quite some time.  Just being paranoid.
	return tmp1;
}


/* TODO: It may be useful to feed into a DRBG, HMAC, or hash to hide the
 * state of the TRNG. Though it probably isn't necessary, because this is a
 * TRNG, it's a good defense in the worst case scenarios.
 */
void random_bytes (uint8_t *dst, uint32_t len)
{
	while (len--)
	{
		*(dst++) = random_uint32 ();
	}
}
