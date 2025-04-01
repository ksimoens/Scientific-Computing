#ifndef GLOBPARAMS
#define GLOBPARAMS

/*
	include the libraries and define global variables
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <math.h>
#include <cmath>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
#include <typeinfo>

#include <hdf5.h>
#include <gsl/gsl_integration.h>

#define CONFIGPATH "config"
// alias, so add brackets for correct division
#define EXACT ((exp(M_PI/2)-1)/2)

#endif