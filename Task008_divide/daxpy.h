#ifndef DAXPY
#define DAXPY

/*

	subfunction that creates dynamic vectors
	function that performs the daxpy vector sum serially
	function that performs the daxpy vector sum in chunks
	function that compares the results of the vector sum
		with the two different methods
	function that serially computes the sum of the elements of a single vector
	function that computes the sum of the elements of a single vector in chunks

*/

#include "globParams.h"

// defined in daxpy.cpp
double** create_vectors(int N,double X,double Y);
double* vector_sum_serial(int N,double a,double** vectors);
double* vector_sum_chunks(int N,int k,double a,double** vectors);
void vector_sum_compare(int N,int k,double a,double X,double Y);
double sum_serial(int n,double* vector);
double sum_chunks(int N,int k,double* vector);


#endif