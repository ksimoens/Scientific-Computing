#include "write.h"

// check if a directory exists
bool check_directory(const char *path){

	struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;

}

// check if a file exists
bool check_file(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

// the function from the exercise (exponential x cosine)
double myfunction(double x){

	return((exp(x)*cos(x)));

}

// create the vectors and write them to a hdf5 file
void write_to_hdf5(struct config_params params){

	// initialise the C hdf5 variables
	hid_t file_id, dsetX_id, dsetF_id, dspace_id, group_id;
	herr_t status;

	// Setup dataset dimensions and input data
	int ndims = 1;
	hsize_t dims[ndims];
	dims[0] = params.N;
	
	// allocate the output vectors
	double* vec_f = new double[params.N];
	double* vec_x = new double[params.N];

	// define the x steps, so that the length of the vectors is N
	// and x starts at 'start' and ends at 'end'
	double dx = (params.end - params.start)/(params.N-1);

	// fill the vectors
	double x = params.start;
	for(int i = 0;i<params.N;i++){
		vec_f[i] = myfunction(x); 
		vec_x[i] = x;
		x += dx;
	}

	// if the output folder does not exist, create it
	if(!check_directory("./output/")){
		std::filesystem::create_directory("./output");
	}

	std::string outname = "./output/"+params.name;

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
	// "X", "F" = name of the data set
	// H5T_NATIVE_DOUBLE = data type
	dsetX_id = H5Dcreate(group_id,"X", H5T_NATIVE_DOUBLE, dspace_id, 
							H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	dsetF_id = H5Dcreate(group_id,"F", H5T_NATIVE_DOUBLE, dspace_id, 
							H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	// Write the data
	// H5S_ALL: the entire space is used for the selected dimensions
	status = H5Dwrite(dsetX_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						vec_x);
	status = H5Dwrite(dsetF_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
						vec_f);
	
	// Close dataset after writing
	status = H5Dclose(dsetX_id);
	status = H5Dclose(dsetF_id);
	status = H5Gclose(group_id);

	// Close the file
	status = H5Fclose(file_id);

	// free memory
	delete[] vec_x;
	delete[] vec_f;

}

// write the results of the integration to the summary CSV
void write_summary(int N, double res_GSL, double res_trap){

	// if the summary CSV does not exist, create it and make header
	if(!check_file("./summary.csv")){
		std::ofstream myfile;
		myfile.open ("summary.csv");
		myfile << "N,iGSL,iTRAP";

		myfile.close();
	}

	// add the current results to the summary CSV
	std::ofstream myfile("summary.csv", std::ios::app);
	myfile.precision(16); // 16-digit precision
	myfile << "\n" << N << "," << res_GSL << "," << res_trap;
	myfile.close();


}