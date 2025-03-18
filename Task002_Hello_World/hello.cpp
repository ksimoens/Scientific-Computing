#include <chrono> // for timing the operations
#include <cmath> // for sqrt
#include <omp.h> // for OpenMP parallel
#include <iostream> // standard

// function to perform the vector sum
double* vector_sum(int N,double a,double x,double y){

	// declare the vectors dynamically
	double* vec_x = new double[N];
	double* vec_y = new double[N];

	// fill the vectors in parallel
	#pragma omp parallel for 
	for(int i=0;i<N;i++){
		vec_x[i] = x;
		vec_y[i] = y;
	}

	double* res = new double[N];

	// calculate the vector sum in parallel
	#pragma omp parallel for 
	for(int i=0;i<N;i++){
		res[i] = a*vec_x[i] + vec_y[i];
	}

	// free memory
	delete[] vec_x;
	delete[] vec_y;

	return(res);

}

// function to check that the vector sum results are as expected
void check_sum(double* res,int N,double a,double x,double y){

	// expected value
	double exp = a*x + y;

	// counter for wrong values
	int c = 0;

	for(int i=0;i<N;i++){

		if(res[i] != exp){
			c++;
		}

	}

	if(c == 0){
		printf("All elements are equal to %.2f.\n",exp);
	} else{
		printf("%i elements are not equal to %.2f.\n",c,exp);
	}

	// free memory
	delete[] res;

}

// function for naieve matrix multiplication
double* matrix_mult(int N,double a,double b){

	double* mat_a = new double[N*N];
	double* mat_b = new double[N*N];

	// fill the matrices in parallel
	#pragma omp parallel for collapse (2)
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			*(mat_a + i * N + j) = a;
			*(mat_b + i * N + j) = b;
		}
	}

	double* res = new double[N*N];

	// naieve implementation = N x N x N operations
	#pragma omp parallel for collapse (2)
	for(int i=0;i<N;i++){
		// switch k and j around to make it more cache friendly
		for(int k=0;k<N;k++){
			for(int j=0;j<N;j++){
				*(res + i * N + j) += *(mat_a + i * N + k) * *(mat_b + k * N + j);
			}
		}
	}

	// free memory
	delete[] mat_a;
	delete[] mat_b;

	return(res);

}

// function for tiled matrix multiplication
double* matrix_mult_block(int N,double a,double b){

	// size of a tile = sqrt(size cache)/(3 matrices)/(size of double)
	int T = 50; //sqrt(3e6)/sizeof(double)/100;

	double* mat_a = new double[N*N];
	double* mat_b = new double[N*N];

	#pragma omp parallel for collapse (2)
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			*(mat_a + i * N + j) = a;
			*(mat_b + j * N + i) = b;
		}
	}

	double* res = new double[N*N];

	#pragma omp parallel for collapse (2)
	// I, J, K are indices over tiles
	for(int I=0;I<N;I+=T){
		for(int J=0;J<N;J+=T){
			for(int K=0;K<N;K+=T){
				// ordinary matrix multiplication for each tile
				for(int i=I;i<I+T;i++){
					for(int j=J;j<J+T;j++){
						double s = 0;
						for(int k=K;k<K+T;k++){
							s += *(mat_a + i * N + k) * *(mat_b + j * N + k);
						}
						*(res + i * N + j) += s;
					}
				}

			}
		}
	}

	// free memory
	delete[] mat_a;
	delete[] mat_b;

	return(res);

}

// function to check that the multiplication result is as expected
void check_mult(double* res,int N,double a,double b){

	// expected result
	double exp = a*b*N;

	int c = 0;

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			// problem, multiplying with N clearly induces floating point errors
			// and the results are no longer exactly the same
			// therefore check whether difference is smaller than 1e-10
			if(*(res + i * N + j) - exp > 1e-10){
				c++;
			}
		}
	}

	if(c == 0){
		printf("All elements are equal to %.2f.\n",exp);
	} else{
		printf("%i elements are not equal to %.2f.\n",c,exp);
	}

	delete[] res;

}

// main function: execute the programme
int main(){
	
	double a = 3.0;
	double x = 0.1;
	double y = 7.1;

	printf("Doing vector sum:\n");

	for(int N: {10,1000000,100000000}){

		// time the operation
		const auto p1 = std::chrono::system_clock::now();
		check_sum(vector_sum(N,a,x,y),N,a,x,y);
		const auto p2 = std::chrono::system_clock::now();
		auto dt = 1e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(p2-p1).count();
		double Nout = N*1.0;
		printf("N = %.2e done in %f seconds.\n",Nout,dt);

	}

	a = 3.0;
	double b = 7.1;

	printf("\nDoing matrix multiplication:\n");

	for(int N: {10,100,10000}){

		const auto p1 = std::chrono::system_clock::now();
		if(N == 10000){
			check_mult(matrix_mult_block(N,a,b),N,a,b);
		} else{
			check_mult(matrix_mult(N,a,b),N,a,b);
		}
		const auto p2 = std::chrono::system_clock::now();
		auto dt = 1e-9*std::chrono::duration_cast<std::chrono::nanoseconds>(p2-p1).count();
		double Nout = N*1.0;
		printf("N = %.2e done in %f seconds.\n",Nout,dt);

	}

	return 0;
}