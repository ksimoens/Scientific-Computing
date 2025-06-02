#ifndef DAXPY
#define DAXPY

/*

	subfunction that creates dynamic vectors
	subfunction that creates GSL vectors
	function that performs the vector sum using the GSL library

*/

#include "globParams.h"

// defined in daxpy.cpp
double** create_vectors(int N,double X,double Y);
gsl_vector** create_GSLvectors(int N,double** vectors);
gsl_vector* vector_sum(double a,gsl_vector** vectors);

#endif