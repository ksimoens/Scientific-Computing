#include "calc.h"

// the function from the exercise with parameters (for GSL)
double mypfunction(double x, void* p){

	return(exp(x)*cos(x));

}

// manuale Trapezoid integration
double integral_trapezoid(struct config_params params){

	// read the vectors with points and function realisations
	double** vectors = read_from_hdf5(params.name);

	// the x spacing
	double dx = vectors[0][1] - vectors[0][0];

	double res = 0.0;

	// do the integration
	for(int i = 0;i<params.N;i++){
		res += dx*(vectors[1][i] + vectors[1][i+1])/2.0;
	}

	// free memory
	delete[] vectors[0];
	delete[] vectors[1];
	// set NULL pointers
	vectors[0] = NULL;
	vectors[1] = NULL;
	vectors = NULL;

	printf("performing integration with manual Trapeziod rule: %.16f\n",res);

	return(res);

}

// read the vectors from the hdf5 file
double** read_from_hdf5(std::string outname){

	// define the C hdf5 parameters
	hid_t file_id, dsetX_id, dsetF_id, dspace_id, group_id;
	herr_t status;
	int ndims;

	std::string dirname = "./output/"+outname;

    // Open the file
    // H5F_ACC_RDONLY = read only
	file_id = H5Fopen(dirname.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	
	// open the data sets (inside the group)
	dsetX_id = H5Dopen(file_id, "/vectors/X", H5P_DEFAULT);
	dsetF_id = H5Dopen(file_id, "/vectors/F", H5P_DEFAULT);

	// get the data space from the data set
	dspace_id = H5Dget_space(dsetX_id);

	// get the dimensions from the data space
	ndims = H5Sget_simple_extent_ndims(dspace_id);
	hsize_t res_dims[ndims];
	status = H5Sget_simple_extent_dims(dspace_id, res_dims, NULL);
	int res_sz = 1;
	for (int i = 0; i < ndims; i++) {
		res_sz *= res_dims[i];
	}
	
	// Retrieve result size and preallocate vector
    double* result_x = new double[res_sz];
    double* result_f = new double[res_sz];
	
    // Read the data
    status = H5Dread(dsetX_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, result_x);
    status = H5Dread(dsetF_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, result_f);

    // Close dataset after writing
	status = H5Dclose(dsetX_id);
	status = H5Dclose(dsetF_id);

	// Close the file
	status = H5Fclose(file_id);

	double** out = new double*[2];
	out[0] = result_x;
	out[1] = result_f;

	return(out);

}

// do the integration with GSL fixed integration
double integral_GSL(struct config_params params){

	double result = 0.0;
	int status;

	// create the GSL function to integrate
	gsl_function F;
	// function field = pointer to function
	F.function = &mypfunction;

	// create the GSL integration workspace
	// gsl_integration_fixed_legendre => all weights = 1
	// 0.0 and 0.0 are determine the weights (ignored for Legendre)
	gsl_integration_fixed_workspace* gsl_wspace = 
		gsl_integration_fixed_alloc(gsl_integration_fixed_legendre, 
									params.N, 
									params.start, 
									params.end,0.0,0.0);

	// do the integration
	// pointer to function and result is stored in result address
	status = gsl_integration_fixed(&F,&result,gsl_wspace);

	// free the memory
	gsl_integration_fixed_free(gsl_wspace);

	printf("performing integration with GSL: %.16f\n",result);

	return(result);

}