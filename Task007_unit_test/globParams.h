#ifndef GLOBPARAMS
#define GLOBPARAMS

/*
	include the libraries and define global variables
*/

#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

#include <gsl/gsl_vector.h>

// the path to the config file
#define CONFIGPATH "config"
// the path to the config file used for testing the reading
#define TESTCONFIGPATH "config_test"
// function used in the unit testing
// if the statement is false, the function returns the function and line
// on which the false statement is generated
#define IS_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

#endif