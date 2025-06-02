#include "globParams.h"
#include "daxpy.h"
#include "read_config.h"

int main(){

	// read the parameters from the configuration file
	struct config_params myparams = read_config(CONFIGPATH);
	
	// compare the vector sum from the two summation methods:
	// serial (as before) and in chunks
	vector_sum_compare(myparams.N,myparams.k,myparams.a,myparams.X,myparams.Y);

	return 0;
}