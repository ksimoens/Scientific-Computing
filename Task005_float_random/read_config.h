#ifndef CONFIG
#define CONFIG

/*
	structure that contains the configuration-file parameters
	function that reads the parameters from the configuration file
*/

#include "globParams.h"

struct config_params{

	int N; // length of the vectors
	std::string name; // output file name

};

// defined in "read_config.cpp"
struct config_params read_config();

#endif