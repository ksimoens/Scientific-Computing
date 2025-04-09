#include "randomGauss.h"

// generate the random numbers, take the sum of the variables and 
// write the result
void generate_numbers(config_params params){

	// allocate the memory for the vectors: 
	// x,y = random Gaussian variables with mean 0 and variance 1
	// d = sum(x,y)
	double** vectors = new double*[3];
	vectors[0] = new double[params.N];
	vectors[1] = new double[params.N];
	vectors[2] = new double[params.N];

	// initialise the random number generator
	// set the time seed = current time
	uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
	// declare the generator
	std::mt19937_64 rng;
	// seed the generator
	rng.seed(timeSeed);
	// declare the distribution from which to draw
	std::normal_distribution<double> dist(0.0, 1.0);

	// fill vectors x and y
	for(int i = 0; i < params.N; i++){
		vectors[0][i] = dist(rng);
		vectors[1][i] = dist(rng);
	}

	// calculate the sum of x and y and store in d
	vector_sum(vectors,params);
	// write the result to an hdf5 file
	write_to_hdf5(vectors,params);

	// free the memory
	delete[] vectors[0];
	delete[] vectors[1];
	delete[] vectors[2];
	delete[] vectors;

}

// take the vector sum of x and y using the GSL library
void vector_sum(double** vectors, config_params params){

	// allocate the GSL vectors
	gsl_vector* gsl_x = gsl_vector_alloc(params.N);
	gsl_vector* gsl_y = gsl_vector_alloc(params.N);

	// fill the GSL vectors
	for (int i = 0; i < params.N; i++){
      gsl_vector_set(gsl_x,i,vectors[0][i]);
      gsl_vector_set(gsl_y,i,vectors[1][i]);
    }

    // do the vector sum:
    // y <- a*x + b*y
    gsl_vector_axpby(1.0,gsl_x,1.0,gsl_y);

    // store the result in the original vectors (d)
    for(int i = 0; i < params.N; i++){
    	vectors[2][i] = gsl_vector_get(gsl_y,i);
    }

    // free the memory
	gsl_vector_free(gsl_x);
	gsl_vector_free(gsl_y);

}

// write the result to an hdf5 file
void write_to_hdf5(double** vectors, config_params params){

	// initialise the C hdf5 variables
	hid_t file_id, dsetX_id, dsetY_id, dsetD_id, dspace_id, group_id;
	herr_t status;

	// Setup dataset dimensions and input data
	int ndims = 1;
	hsize_t dims[ndims];
	dims[0] = params.N;

	// Open a file
	// H5F_ACC_TRUNC: if the file exists, it will be overwritten
	// H5P_DEFAULT default value for the requested type
	file_id = H5Fcreate(params.name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

	// Create a group
	group_id = H5Gcreate2(file_id, "/vectors", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// Create a dataspace
	// 1 = rank = number of dimensions
	// dims = the size of each dimension
	// NULL = the maximum size of each dimension
	dspace_id = H5Screate_simple(1, dims, NULL);

	// create the data sets
	// "X", "Y" = name of the data set
	// H5T_NATIVE_DOUBLE = data type
	dsetX_id = H5Dcreate(group_id,"X", H5T_NATIVE_DOUBLE, dspace_id, 
							H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	dsetY_id = H5Dcreate(group_id,"Y", H5T_NATIVE_DOUBLE, dspace_id, 
							H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	dsetD_id = H5Dcreate(group_id,"D", H5T_NATIVE_DOUBLE, dspace_id, 
							H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// Write the data
	// H5S_ALL: the entire space is used for the selected dimensions
	status = H5Dwrite(dsetX_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						vectors[0]);
	status = H5Dwrite(dsetY_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						vectors[1]);
	status = H5Dwrite(dsetD_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						vectors[2]);
	
	// Close dataset after writing
	status = H5Dclose(dsetX_id);
	status = H5Dclose(dsetY_id);
	status = H5Gclose(group_id);

	// Close the file
	status = H5Fclose(file_id);

}