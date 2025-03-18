# Hello World in Algebra

## Julia

Here you can find the source code for the exercise in Julia:  
To run the code use:  
```
$ julia hello.jl
```  

```
# load necassary modules
using Printf # for C-style printing

# function to calculate the vector sum
function vector_sum(N,a,x,y)

	vec_x = ones(N)*x
	vec_y = ones(N)*y

	return(a*vec_x .+ vec_y)

end

# function to calculate the matrix multiplication
function matrix_mult(N,a,b)

	mat_a = ones((N,N))*a
	mat_b = ones((N,N))*b

	return(mat_a * mat_b)

end	

# function to check if all elements are equal to the expected one
# multiple broadcasting: for vector sum
function check_elem(res,a,x,y)

	check = length(findall(z->(z != a*x+y),res))
	if(check==0)
		@printf("All elements are equal to %.2f.\n",a*x+y)
	else
		@printf("%i elements are not equal to %.2f.\n",check,a*x+y)
	end

end

# multiple broadcasting: for matrix multiplication
function check_elem(res,a,b)

	N = size(res)[1]

	# problem, multiplying with N clearly induces floating point errors
	# and the results are no longer exactly the same
	# therefore check whether difference is smaller than 1e-10
	check = length(findall(z->(abs(z/N - a*b) > 1e-10),res))
	if(check==0)
		@printf("All elements are equal to %.2f.\n",a*b*N)
	else
		@printf("%i elements are not equal to %.2f.\n",check,a*b*N)
	end

end

# main function
function main()
	
	# ------ VECTOR SUM -------
	a = 3.0
	x = 0.1
	y = 7.1
	
	@printf("Doing vector sum:\n")

	# for lengths 10, 1000000, 100000000
	for N in [10,round(Int64,1e6),round(Int64,1e8)]

		# get the time it takes to run the operation and the check
		t = time()
		check_elem(vector_sum(N,a,x,y),a,x,y)
		@printf("N = %.2e done in %f seconds.\n",N,time()-t)

	end

	# -------------------------

	# - MATRIX MULTIPLICATION -
	a = 3.0
	b = 7.1

	@printf("\nDoing matrix multiplication:\n")

	for N in [10,round(Int64,1e2),round(Int64,1e4)]

		t = time()
		check_elem(matrix_mult(N,a,b),a,b)
		@printf("N = %.2e done in %f seconds.\n",N,time()-t)

	end

	# -------------------------

end

main() # execute the main function
```

The code creates the necessary arrays, performs the operations and checks the result for each size parameter.  
The operations are also timed.  

### Problems and Remarks

The vector sum can be executed without any problems for all size parameters considered. The broadcasting `.` option probably performs the sum very efficiently under the hood.  
Checking of the result is equally efficient via the `findall` function.  
  
Remarkably, the matrix multiplication poses also no problems in terms of efficiency. Calculating the multiplication with the `*` operator returns the result for N=10,000 in a matter of seconds.  
When checking the result, multiplying the expected value with N, probably induces floating point errors when subtracting it from the calculated value. The values are therefore not exactly identical. Changing the equality with testing whether their difference becomes very small fixes this problem.

## C++

Here you can find the source code for the exercise in C++:  
To compile the code use:  
```
$ g++ -fopenmp hello.cpp -o hello
```  
and execute the programme as:
```
$ ./hello
```

```
#include <chrono> // for timing the operations
#include <cmath> // for sqrt
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
			// transpose the matrix B for memory efficiency
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
	
	// ------ VECTOR SUM -------
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

	// -------------------------

	// - MATRIX MULTIPLICATION -

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

	// -------------------------

	return 0;
}
```  

### Problems and Remarks

The vector sum again runs smoothly with runtimes similar to the ones in Julia.  
Arrays of length 1,000,000 cannot be declared statically in memory and have to be declared dynamically. Memory leaks have to be avoided by always freeing this allocated memory.  
  
The matrix multiplication becomes problematic for large arrays as it scales with a cubic power. Moreover, the row-major character of C makes that elements in the second matrix have to be fetched from RAM memory at every operation. Transposing this matrix and rearranging the loops optimises the use of cache memory.  
  
Despite these measures, multiplying two matrices of size N=10,000 would take hours to run. Further improvements are made by tiling the matrices such that each piece fits in the RAM memory. Additionally, the operations are parallellised using OpenMP. Multiplying matrices of size N=10,000 now takes ten minutes.  
This only puts emphasis on the remarkable efficiency of Julia, which likely uses highly optimised algorithms under the hood.