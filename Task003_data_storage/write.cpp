#include "write.h"

void write_to_hdf5(int N, double X, double Y, std::string outname){

	// initialise the C hdf5 variables
	hid_t file_id, dsetX_id, dsetY_id, dspace_id, group_id;
	herr_t status;

	// Setup dataset dimensions and input data
	int ndims = 1;
	hsize_t dims[ndims];
	dims[0] = N;
	
	// simulate data coming from dynamic arrays
	double* vec_x = new double[N];
	double* vec_y = new double[N];
	for(int i = 0;i<N;i++){
		vec_x[i] = X;
		vec_y[i] = Y;
	}

	// Open a file
	// H5F_ACC_TRUNC: if the file exists, it will be overwritten
	// H5P_DEFAULT default value for the requested type
	file_id = H5Fcreate(outname.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

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

	// Write the data
	// H5S_ALL: the entire space is used for the selected dimensions
	status = H5Dwrite(dsetX_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						vec_x);
	status = H5Dwrite(dsetY_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						vec_y);
	
	// Close dataset after writing
	status = H5Dclose(dsetX_id);
	status = H5Dclose(dsetY_id);
	status = H5Gclose(group_id);

	// Close the file
	status = H5Fclose(file_id);

	delete[] vec_x;
	delete[] vec_y;

}