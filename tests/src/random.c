#include <stdio.h>
#include <minunit.h>
#include <strong-arm/random.h>


/* Count the number of ones to check for bias */
START_TEST (test_bitcount)
{
	const uint32_t iterations = 1000000;	// Do not set higher than ~134,000,000
	const uint32_t subcount[16] = {0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4};
	uint32_t ones = 0, tmp;
	
	/* Discard first result */
	random_uint32 ();
	
	for (uint32_t i = 0; i < iterations; ++i)
	{
		tmp = random_uint32 ();
		ones += subcount[(tmp >> 0) & 15];
		ones += subcount[(tmp >> 4) & 15];
		ones += subcount[(tmp >> 8) & 15];
		ones += subcount[(tmp >> 12) & 15];
		ones += subcount[(tmp >> 16) & 15];
		ones += subcount[(tmp >> 20) & 15];
		ones += subcount[(tmp >> 24) & 15];
		ones += subcount[(tmp >> 28) & 15];
	}
	
	uint32_t expected = iterations << 4;
	uint32_t error = (ones > expected) ? (ones - expected) : (expected - ones);
	printf ("Random Test: Bitcount\nOnes: %lu (expected %lu)\n", ones, expected);
	printf ("Error: %f%%\n\n", (error * 100.0f) / expected);
}
END_TEST


void test_wordcount (uint32_t bits)
{
	const uint32_t iterations = 1000000;	// Do not set higher than ~134,000,000
	const uint32_t mask = (1 << bits) - 1;
	uint32_t buckets[32] = {0};
	uint32_t tmp;
	
	/* Discard first result */
	random_uint32 ();
	
	for (uint32_t i = 0; i < iterations; ++i)
	{
		tmp = random_uint32 ();
		
		for (uint32_t j = 0; j < 32 / bits; ++j)
			buckets[(tmp >> (j * bits)) & mask] += 1;
	}
	
	uint32_t expected = ((32 / bits) * iterations) >> bits;
	
	printf ("Random Test: Wordcount (k=%lu)\n", bits);
	uint32_t maxerror = 0;
	for (uint32_t i = 0; i < (1 << bits); ++i)
	{
		uint32_t error = (buckets[i] > expected) ? (buckets[i] - expected) : (expected - buckets[i]);
		printf ("[%2lu] = %lu (expected %lu)\n", i, buckets[i], expected);
		
		if (error > maxerror)
			maxerror = error;
	}
	
	printf ("Max Error: %f%%\n\n", (maxerror * 100.0f) / expected);
}


char *test_random (void)
{
	mu_run_test (test_bitcount);
	test_wordcount (2);
	test_wordcount (3);
	test_wordcount (4);
	test_wordcount (5);
	
	return 0;
}
