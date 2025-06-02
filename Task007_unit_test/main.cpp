#include "globParams.h"
#include "daxpy.h"
#include "read_config.h"
#include "test.h"

int main(){

	// unit tests
	// test the reading of parameters
	test_read();
	// test that the created vectors have the correct value
	test_vector_values();
	// test that the values are correctly transferred to GSL vectors
	test_GSLvector_values();
	// test the GSL vector sum
	test_vector_sum();

	// read the parameters from the configuration file
	struct config_params myparams = read_config(CONFIGPATH);
	// perform the vector sum and store in a gsl_vector
	gsl_vector* res = vector_sum(myparams.a,
							create_GSLvectors(myparams.N,
								create_vectors(myparams.N,myparams.X,myparams.Y)
							)
						);

	std::cout << "the result of the vector sum is: " << 
						gsl_vector_get(res,0) << std::endl;

	gsl_vector_free(res);

	return 0;
}