#ifndef __MINUNIT_H__
#define __MINUNIT_H__

#define mu_assert(test, message) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
#define START_TEST(test) static char *test (void) {
#define END_TEST return 0;}

extern int tests_run;

#endif

