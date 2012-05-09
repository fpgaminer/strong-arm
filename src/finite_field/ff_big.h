#ifndef __PRIVATE_FF_BIG_H__
#define __PRIVATE_FF_BIG_H__

#include <stdint.h>
#include <finite_field.h>

/* A large FF number; used internally by the multiplication algorithm. */
typedef struct {
	uint32_t z[16];
} FF_NUM_BIG;

void _ff_big_mod (FF_NUM *const out, FF_NUM_BIG const *const a, FF_NUM const *const p);

#endif

