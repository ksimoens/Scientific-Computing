#ifndef CALC
#define CALC

/*
	function that reads the initial vectors from the output file
	function that performs the vector sum with GSL
	function that writes the vector sum to the output file
*/

#include "globParams.h"

// defined in "calc.cpp"
double** read_from_hdf5(std::string outname);
gsl_vector* vector_sum(double** vectors,double a,int N);
void write_result_hdf5(std::string outname, int N, gsl_vector* gsl_y);

#endif