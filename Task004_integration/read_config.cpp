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
		if(param == "N"){
			std::string N0;
			in >> N0;
			myparams.N = std::stoi(N0,&pos);
		// the output file name
		} else if(param == "outname"){
			std::string name;
			in >> name;
			myparams.name = name+"_N"+std::to_string(myparams.N)+".h5";
		// the lower end of the integration interval
		} else if(param == "start"){
			std::string s0;
			in >> s0;
			myparams.start = std::stod(s0,&pos);
		// the higher end of the integration interval (work with pi)
		} else if(param == "end"){
			std::string e0;
			in >> e0;
			if(e0 == "3.14"){
				myparams.end = M_PI;
			} else if(e0 == "1.57"){
				myparams.end = M_PI/2.0;
			} else{
				myparams.end = std::stod(e0,&pos);
			}
		}

	}

	return(myparams); 

}