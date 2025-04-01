#ifndef WRITE
#define WRITE

/*
	function that writes the vectors to the output file
*/

#include "globParams.h"
#include "read_config.h" // necessary for the config_params structure

// defined in "write.cpp"
void write_to_hdf5(struct config_params params);
double myfunction(double x);
bool check_directory(const char *path);
bool check_file(const std::string& name);
void write_summary(int N, double res_GSL, double res_trap);

#endif