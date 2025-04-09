/*
	main C++ execution
*/

#include "globParams.h"
#include "floatSum.h"
#include "read_config.h"
#include "randomGauss.h"

int main(int argc, char *argv[]){

	// ****** SUM ******
	// perform the sum via for loop
	double loop_sum = loopSum();
	printf("the result of the sum with for loop is %.16f\n",loop_sum);
	// perform the sum via GSL vector sum
	double gsl_sum = gslSum();
	printf("the result of the sum with GSL vector is %.16f\n",gsl_sum);
	// perform the sum via the Kahan algorithm
	double kahan_sum = kahanSum();
	printf("the result of the sum with Kahan algorithm is %.16f\n",kahan_sum);

	// ****** RANDOM NUMBER ******
	// read the parameters
	config_params params = read_config();
	std::cout << "random numbers will be stored in " << params.name << std::endl;
	// generate the random numbers, perform the sum and store the result
	generate_numbers(params);
	std::cout << "random numbers generated" << std::endl;

	return 0;
}