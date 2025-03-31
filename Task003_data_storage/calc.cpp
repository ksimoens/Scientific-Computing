#include "calc.h"

double** read_from_hdf5(std::string outname){

	// define the C hdf5 parameters
	hid_t file_id, dsetX_id, dsetY_id, dspace_id, group_id;
	herr_t status;
	int ndims;

    // Open the file
    // H5F_ACC_RDONLY = read only
	file_id = H5Fopen(outname.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
	
	// open the data sets (inside the group)
	dsetX_id = H5Dopen(file_id, "/vectors/X", H5P_DEFAULT);
	dsetY_id = H5Dopen(file_id, "/vectors/Y", H5P_DEFAULT);

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
    double* result_y = new double[res_sz];
	
    // Read the data
    status = H5Dread(dsetX_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, result_x);
    status = H5Dread(dsetY_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, result_y);

    // Close dataset after writing
	status = H5Dclose(dsetX_id);
	status = H5Dclose(dsetY_id);

	// Close the file
	status = H5Fclose(file_id);

	double** out = new double*[2];
	out[0] = result_x;
	out[1] = result_y;

	return(out);

}

gsl_vector* vector_sum(double** vectors,double a, int N){

	std::cout << "performing vector sum with GSL" << std::endl;

	// allocate the GSL vectors
	gsl_vector* gsl_x = gsl_vector_alloc(N);
	gsl_vector* gsl_y = gsl_vector_alloc(N);

	// fill the GSL vectors
	for (int i = 0; i < N; i++){
      gsl_vector_set(gsl_x,i,vectors[0][i]);
      gsl_vector_set(gsl_y,i,vectors[1][i]);
    }

    // do the vector sum:
    // y <- a*x + b*y
    gsl_vector_axpby(a,gsl_x,1.0,gsl_y);

    // free the memory
	gsl_vector_free(gsl_x);

	delete[] vectors[0];
	delete[] vectors[1];
	delete[] vectors;

	// return the result
	return(gsl_y);

}

void write_result_hdf5(std::string outname, int N, gsl_vector* gsl_y){

	// initialise the C hdf5 parameters
	hid_t file_id, dset_id, dsetX_id, dspace_id, group_id;
	herr_t status;
	int ndims;

	std::cout << "writing vector sum to " << outname << std::endl;

	// Open a file
	// H5F_ACC_RDWR = read/write access
	file_id = H5Fopen(outname.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

	// get an original data set for dimension extraction
	dsetX_id = H5Dopen(file_id, "/vectors/X", H5P_DEFAULT);

	// define the group for the new data
	group_id = H5Gopen(file_id, "/vectors", H5P_DEFAULT);

	// extract the dimensional information from the original data set
	dspace_id = H5Dget_space(dsetX_id);
	ndims = H5Sget_simple_extent_ndims(dspace_id);
	hsize_t res_dims[ndims];
	status = H5Sget_simple_extent_dims(dspace_id, res_dims, NULL);

	// create the new data set for the vector sum
	dset_id = H5Dcreate(group_id,"SUM", H5T_NATIVE_DOUBLE, dspace_id, 
							H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	// write the result to the new data set
	status = H5Dwrite(dset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						gsl_y->data);
	
	// Close dataset after writing
	status = H5Dclose(dsetX_id);
	status = H5Dclose(dset_id);
	status = H5Gclose(group_id);

	// Close the file
	status = H5Fclose(file_id);

	// free the memory
	gsl_vector_free(gsl_y);

}