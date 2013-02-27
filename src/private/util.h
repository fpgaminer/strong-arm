#ifndef __PRIVATE_UTIL_H__
#define __PRIVATE_UTIL_H__

#include <stdint.h>

/* Enter an error state that cannot be exitted and which freezes the device.
 * This is used by the cryptographic functions when something goes horribly wrong.
 * This is recommended by FIPS 140. */
void error_state (void);

/* Binary Logarithm */
uint32_t blog2 (uint32_t a);

#endif
