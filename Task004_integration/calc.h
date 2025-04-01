#ifndef CALC
#define CALC

/*
	function that reads the initial vectors from the output file
	function that performs the Trapezoid integration 
	function that performs the integration with GSL
	function that contains the function from the exercise
*/

#include "globParams.h"
#include "read_config.h" // for the config_params structure

// defined in "calc.cpp"
double** read_from_hdf5(std::string outname);
double integral_trapezoid(struct config_params params);
double integral_GSL(struct config_params params);
double mypfunction(double x,void* p);

#endif