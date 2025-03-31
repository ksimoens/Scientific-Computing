#ifndef WRITE
#define WRITE

/*
	function that writes the initial vectors to the output file
*/

#include "globParams.h"

// defined in "write.cpp"
void write_to_hdf5(int N, double X, double Y, std::string outname);

#endif