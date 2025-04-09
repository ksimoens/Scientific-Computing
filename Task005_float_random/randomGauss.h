#ifndef RANDOMGAUSS
#define RANDOMGAUSS

/* 
	performing the random-number exercise
	generate random Gaussian numbers
	do the sum of two variables
	write the result to an hdf5 file
*/

#include "globParams.h"
#include "read_config.h"

// defined in "randomGauss.cpp"
void generate_numbers(config_params params);
void vector_sum(double** vectors,config_params params);
void write_to_hdf5(double** vectors, config_params params);


#endif