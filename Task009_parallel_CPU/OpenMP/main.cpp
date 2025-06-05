#include "globParams.h"
#include "read_config.h"

// dummy function that fills a vector with a single value 
void fill_vector(int N,double* vector,double value){

	for(int i=0;i<N;i++){
		vector[i] = value;
	}

}

// everything is written in the main function
// in order to maintain an overview over the OpenMP implementation
int main(){

	// read the parameters from the configuration file
	struct config_params myparams = read_config(CONFIGPATH);

	// set the number of threads
	omp_set_num_threads(myparams.c);

	// allocate memory for the global vectors and fill them
	double* vecX = new double[myparams.N];
	double* vecY = new double[myparams.N];
	double* vecZ_serial = new double[myparams.N];
	double* vecZ_parall = new double[myparams.N];
	fill_vector(myparams.N,vecX,myparams.X);
	fill_vector(myparams.N,vecY,myparams.Y);

	// summation variables
	double sum_serial = 0.0;
	double sum_parall = 0.0;

	// timer variables
	double dt_serial;
	double dt_parall;
	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;

	// do the serial summation
	start = std::chrono::system_clock::now();
	for(int i=0;i<myparams.N;i++){
		vecZ_serial[i] = myparams.a*vecX[i] + vecY[i];
	}
	// perform the element summation separately to compare with parallel one
	for(int i=0;i<myparams.N;i++){
		sum_serial += vecZ_serial[i];
	}
	end = std::chrono::system_clock::now();
	dt_serial = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	// do the parallel summation
	start = std::chrono::system_clock::now();
	#pragma omp parallel for
	for(int i=0;i<myparams.N;i++){
		vecZ_parall[i] = myparams.a*vecX[i] + vecY[i];
	}
	#pragma omp parallel for reduction (+:sum_parall)
	for(int i=0;i<myparams.N;i++){
		sum_parall += vecZ_parall[i];
	}
	end = std::chrono::system_clock::now();
	dt_parall = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	// check whether the serial and parallel results are the same
	bool correct = true;
	for(int i=0;i<myparams.N;i++){
		correct *= (vecZ_serial[i] == vecZ_parall[i]);
	}

	IS_TRUE(correct);
	IS_TRUE(sum_serial == sum_parall);
	IS_TRUE((sum_serial - sum_parall) < 1e-16);

	// print the outcome
	std::cout << "daxpy vector sum with OpenMP" << std::endl;
	std::cout << "OpenMP run using " << omp_get_max_threads() << " threads" << std::endl;
	std::cout << "vectors of size " << myparams.N << std::endl;
	std::cout << "serial runtime is: " << dt_serial << " ns" << std::endl;
	std::cout << "parallel runtime is: " << dt_parall << " ns" << std::endl;
	std::cout << "final result of the sum is: " << vecZ_serial[0] << std::endl;
	std::cout << "the sum of the vector sum is: " << sum_serial << " for the serial process" << std::endl;
	std::cout << "the sum of the vector sum is: " << sum_parall << " for the parallel process" << std::endl;
	
	// free the global memory
	delete[] vecX;
	delete[] vecY;
	delete[] vecZ_serial;
	delete[] vecZ_parall;

	return 0;
}