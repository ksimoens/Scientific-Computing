#include "globParams.h"
#include "write.h"
#include "calc.h"
#include "read_config.h"

int main(int argc, char *argv[]){

	// read the parameters from the command line
	std::size_t pos;
	int N = std::stoi(argv[1], &pos); // length of the vectors
	double X = std::stod(argv[2], &pos); // value of the X vector
	double Y = std::stod(argv[3], &pos); // value of the Y vector
	std::cout << "vector length read as " << N << std::endl;

	// read the parameters from the configuration file
	struct config_params myparams = read_config();

	std::cout << "multiplication factor read as " << myparams.a << std::endl;
	// name of the hdf5 output file
	std::string outname = myparams.name + "_N" + std::to_string(N) + ".h5";

	// write the initial vectors to the output file
	write_to_hdf5(N,X,Y,outname);
	// read the initial vectors from the output file
	// perform the vector sum using GSL
	// write the result to the output file
	write_result_hdf5(outname,N,vector_sum(read_from_hdf5(outname),myparams.a,N));

	return 0;
}