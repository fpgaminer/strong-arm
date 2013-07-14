#ifndef __FINITE_FIELD_Low_LEVEL_H__
#define __FINITE_FIELD_Low_LEVEL_H__


#if defined (__arm__)
	#include "low_level_arm.inc"
#else
	#include "low_level_generic.inc"
#endif


#endif
