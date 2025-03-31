#include "read_config.h"

struct config_params read_config(){

	// open the configuration file
	std::ifstream in(CONFIGPATH);

	// variables for the parameter name
	std::string param;

	std::size_t pos;

	// define the structure for the configuration parameters
	struct config_params myparams;

	// read the lines in the configuration file
	while(!in.eof()){

		// read the first word on the line
		in >> param;

		// the coeficcient of the vector sum
		if(param == "a"){
			std::string a0;
			in >> a0;
			myparams.a = std::stoi(a0,&pos);
		// the output file name
		} else if(param == "outname"){
			std::string name;
			in >> name;
			myparams.name = name;
		}

	}

	return(myparams); 

}