#include "read_config.h"

struct config_params read_config(std::string path){

	// open the configuration file
	std::ifstream in(path);

	// variables for the parameter name
	std::string param;

	std::size_t pos;

	// define the structure for the configuration parameters
	struct config_params myparams;

	// read the lines in the configuration file
	while(!in.eof()){

		// read the first word on the line
		in >> param;

		// the length of the vectors
		if(param == "N"){
			std::string N0;
			in >> N0;
			myparams.N = std::stoi(N0,&pos);
		// the coefficient of the vector sum
		} else if(param == "a"){
			std::string a0;
			in >> a0;
			myparams.a = std::stod(a0,&pos);
		// the values of the X vector
		} else if(param == "X"){
			std::string X0;
			in >> X0;
			myparams.X = std::stod(X0,&pos);
		// the values of the Y vector
		} else if(param == "Y"){
			std::string Y0;
			in >> Y0;
			myparams.Y = std::stod(Y0,&pos);
		}

	}

	return(myparams); 

}