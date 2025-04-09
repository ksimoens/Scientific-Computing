#ifndef FLOATSUM
#define FLOATSUM

/*
	perform the sum of the floating points
	sum via a for loop
	sum via the GSL library
	sum via the Kahan algorithm
*/

#include "globParams.h"

// defined in "floatSum.cpp"
double loopSum();
double gslSum();
double kahanSum();

#endif