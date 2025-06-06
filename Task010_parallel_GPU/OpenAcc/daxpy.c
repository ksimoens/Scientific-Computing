#include <stdio.h>
#include <openacc.h>
#include <stdlib.h>
#include <time.h>

// define the variable
#define X 0.1
#define Y 7.1
#define a 3.0
#define N 100000

// everything is written in the main function
// in order to maintain an overview over the OpenAcc implementation
int main(){

	// allocate memory for the global vectors and fill them
	double* vecX;
	double* vecY;
	double* vecZ;
	vecX = (double*) malloc(N*sizeof(double));
	vecY = (double*) malloc(N*sizeof(double));
	vecZ = (double*) malloc(N*sizeof(double));

	// fill the vectors
	for(int i=0;i<N;i++){
		vecX[i] = X;
		vecY[i] = Y;
	}

	// timer variables
	double dt_acc;
	clock_t start;
	clock_t end;

	
	// do the parallel summation with OpenAcc
	start = clock();

	#pragma acc parallel loop copyin(vecX[0:N], vecY[0:N]) copyout(vecZ[N])
	for(int i=0;i<N;i++){
		vecZ[i] = a*vecX[i] + vecY[i];
	}

	end = clock();
	dt_acc = (end - start)*1000/(double)CLOCKS_PER_SEC;

	// check if OpenAcc is using the GPU
	int NUM = 1000;
	int* buf;
	buf = (int*) malloc(NUM*sizeof(int));
	for(int i=0;i<NUM;i++){
		buf[i] = 0;
	}

	#pragma acc parallel create(buf[NUM])
	for(int i=0;i<NUM;i++){
		buf[i] = 1;
	}

	if (buf[0] == 1) {
        printf("Running on CPU\n");
    } else {
        printf("Running on GPU\n");
    }

	// print the outcome
	printf("daxpy vector sum with OpenAcc\n");
	printf("vectors of size %i\n",N);
	printf("OpenAcc runtime is: %f ms\n",dt_acc);
	printf("final result of the sum is: %.2f\n",vecZ[0]);
	
	// free the global memory
	free(vecX);
	free(vecY);
	free(vecZ);
	free(buf);

	return 0;
}