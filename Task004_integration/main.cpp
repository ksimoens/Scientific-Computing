/*
	main C++ execution
*/

#include "globParams.h"
#include "write.h"
#include "calc.h"
#include "read_config.h"

int main(int argc, char *argv[]){

	// read the parameters from the configuration file
	struct config_params myparams = read_config();

	std::cout << "number of sampling points read as " << myparams.N << std::endl;
	std::cout << "start of integration region read as " << myparams.start << std::endl;
	if(myparams.end == M_PI){
		std::cout << "end of integration region read as π" << std::endl;
	} else if(myparams.end == M_PI/2.0){
		std::cout << "end of integration region read as π/2" << std::endl;
	} else{
		printf("end of integration region read as %.2f\n",myparams.end);
	}
	// name of the hdf5 output file
	std::cout << "output stored in " << myparams.name << std::endl;

	// write the vectors to the output file
	write_to_hdf5(myparams);
	
	// do the numerical integration with GSL 
	// commented out for efficiency of the full programme
	double res_GSL = EXACT;//integral_GSL(myparams);

	// do the numerical integration with the Trapezoid rule
	double res_trap = integral_trapezoid(myparams);

	// print the results up to 16 digits
	printf("the exact solution is: %.16f\n",EXACT);
	printf("relative error manual Trapezoid rule: %.16f\n",res_trap/EXACT-1);
	printf("relative error GSL integration: %.16f\n",res_GSL/EXACT-1);

	// write the result to the summary CSV file
	write_summary(myparams.N,res_GSL,res_trap);

	return 0;
}