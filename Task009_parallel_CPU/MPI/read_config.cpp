#include "read_config.h"

struct config_params read_config(std::string path){

	// open the configuration file
	std::ifstream in(path);

	// variables for the parameter name
	std::string line;

	std::size_t pos;

	// define the structure for the configuration parameters
	struct config_params myparams;

	// read the lines in the configuration file
	//while(!in.eof()){
	while(getline( in, line )){

		std::stringstream ss(line);
		std::string s;
		std::string params[2];

		int i=0;
		while (getline(ss, s, ' ')) {
		// store token string in the vector
        	params[i] = s;
        	i++;
    	}

		// the length of the vectors
		if(params[0] == "N"){
			myparams.N = std::stoi(params[1],&pos);
		// the coefficient of the vector sum
		} else if(params[0] == "a"){
			myparams.a = std::stod(params[1],&pos);
		// the values of the X vector
		} else if(params[0] == "X"){
			myparams.X = std::stod(params[1],&pos);
		// the values of the Y vector
		} else if(params[0] == "Y"){
			myparams.Y = std::stod(params[1],&pos);
		// the number of threads for OpenMP
		} else if(params[0] == "c"){
			myparams.c = std::stoi(params[1],&pos);
		}

	}

	return(myparams); 

}