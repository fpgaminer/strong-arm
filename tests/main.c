#include <stdio.h>
#include <finite_field.h>

#define mu_assert(test, message) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
#define START_TEST(test) static char *test (void) {
#define END_TEST return 0;}

int tests_run = 0;


START_TEST (test_compare)
{
	const FF_NUM a = {0xDEADBEEF, 0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210, 0x77777777, 0x39103739, 0xFFFFFFFE};
	const FF_NUM b = {0};
	const FF_NUM c = {1, 0, 0, 0, 0, 0, 0, 0};
	
	mu_assert (ff_compare (&a, &a) == 0, "ff_compare should return 0 if the arguments are equal.");
	mu_assert (ff_compare (&a, &b) > 0, "ff_compare should return >0 if the first argument is greater than the second.");
	mu_assert (ff_compare (&b, &c) < 0, "ff_compare should return <0 if the first argument is less than the second.");
}
END_TEST

START_TEST (test_copy)
{
	const FF_NUM a = {0xDEADBEEF, 0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210, 0x77777777, 0x39103739, 0xFFFFFFFE};
	const FF_NUM b = {0xDEADBEEF, 0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210, 0x77777777, 0x39103739, 0xFFFFFFFE};
	FF_NUM c = {0};
	
	ff_copy (&c, &a);
	mu_assert (ff_compare (&a, &b) == 0, "ff_copy should not modify the source argument.");
	mu_assert (ff_compare (&a, &c) == 0, "ff_copy should create a duplicate copy of the source in the destination.");
	ff_copy (&c, &c);
	mu_assert (ff_compare (&c, &b) == 0, "ff_copy should allow source and destination to point to the same FF_NUM.");
}
END_TEST

// TODO: Exercise more corner cases (overflows, negative values on subtraction, etc).
START_TEST (test_add)
{
	FF_NUM d, e;

	const FF_NUM a = {0x57aea72f, 0x683bdd9e, 0x0dd1423b, 0x3b452e51, 0x34c36974, 0xfe062139, 0x4e56ed4d, 0x80863fef};
	const FF_NUM b = {0x10b4944e, 0x09afc2ca, 0x8bddaccb, 0xfbc985b9, 0xe6ff3acd, 0x02ba734b, 0x9ab70589, 0x69f64c21};
	const FF_NUM p1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	const FF_NUM p2 = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x90000000};
	const FF_NUM p3 = {0x00000042, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
	const FF_NUM r1 = {0x68633B7D, 0x71EBA068, 0x99AEEF06, 0x370EB40A, 0x1BC2A442, 0x00C09485, 0xE90DF2D7, 0xEA7C8C10};
	const FF_NUM r2 = {0x68633B7D, 0x71EBA068, 0x99AEEF06, 0x370EB40A, 0x1BC2A442, 0x00C09485, 0xE90DF2D7, 0x5A7C8C10};
	const FF_NUM r3 = {0xD0C676FB, 0xE3D740D0, 0x335DDE0C, 0x6E1D6815, 0x37854884, 0x0181290A, 0xD21BE5AE, 0xD4F91821};
	const FF_NUM r4 = {0x0000002F, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

	ff_add (&d, &a, &b, &p1);
	ff_add (&e, &a, &b, &p2);
	mu_assert (ff_compare (&d, &r1) == 0, "ff_add should add arguments (mod 2^256-1).");
	mu_assert (ff_compare (&e, &r2) == 0, "ff_add should add arguments (mod p).");

	ff_add (&d, &d, &d, &p1);
	mu_assert (ff_compare (&d, &r3) == 0, "ff_add should allow the destination argument to be the same as the source arguments.");

	ff_add (&d, &a, &b, &p3);
	mu_assert (ff_compare (&d, &r4) == 0, "ff_add should add arguments (mod p) even if the arguments are greater than p.");
}
END_TEST

// TODO: Exercise more corner cases (overflows, negative values on subtraction, etc).
START_TEST (test_sub)
{
	FF_NUM d, e;

	const FF_NUM a = {0x57aea72f, 0x683bdd9e, 0x0dd1423b, 0x3b452e51, 0x34c36974, 0xfe062139, 0x4e56ed4d, 0x80863fef};
	const FF_NUM b = {0x10b4944e, 0x09afc2ca, 0x8bddaccb, 0xfbc985b9, 0xe6ff3acd, 0x02ba734b, 0x9ab70589, 0x99f64c21};
	const FF_NUM p1 = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};
	const FF_NUM p2 = {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x90000000};
	const FF_NUM p3 = {0x00000042, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};
	const FF_NUM r1 = {0x46FA12E0, 0x5E8C1AD4, 0x81F39570, 0x3F7BA897, 0x4DC42EA6, 0xFB4BADED, 0xB39FE7C4, 0xE68FF3CD};
	const FF_NUM r2 = {0x46FA12E1, 0x5E8C1AD4, 0x81F39570, 0x3F7BA897, 0x4DC42EA6, 0xFB4BADED, 0xB39FE7C4, 0x768FF3CD};
	const FF_NUM r4 = {0x00000029, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

	ff_sub (&d, &a, &b, &p1);
	ff_sub (&e, &a, &b, &p2);
	mu_assert (ff_compare (&d, &r1) == 0, "ff_sub should subtract arguments (mod 2^256-1).");
	mu_assert (ff_compare (&e, &r2) == 0, "ff_sub should subtract arguments (mod p).");

	ff_sub (&d, &d, &d, &p1);
	mu_assert (ff_is_zero (&d), "ff_sub should allow the destination argument to be the same as the source arguments.");

	ff_sub (&d, &a, &b, &p3);
	mu_assert (ff_compare (&d, &r4) == 0, "ff_sub should subtract arguments (mod p) even if the arguments are greater than p.");
}
END_TEST


static char *all_tests ()
{
	mu_run_test (test_compare);
	mu_run_test (test_copy);
	mu_run_test (test_add);
	mu_run_test (test_sub);
	
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
