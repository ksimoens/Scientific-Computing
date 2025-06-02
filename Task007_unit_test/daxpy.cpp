#include "daxpy.h"

double** create_vectors(int N,double X,double Y){

	// dynamically allocate the vectors
	double* result_x = new double[N];
    double* result_y = new double[N];

    // fill the vectors
    for(int i=0;i<N;i++){
    	result_x[i] = X;
    	result_y[i] = Y;
    }

    // pointer to pointers to doubles
    double** out = new double*[2];
	out[0] = result_x;
	out[1] = result_y;

	return(out);

}

gsl_vector** create_GSLvectors(int N,double** vectors){

	// allocate the GSL vectors
	gsl_vector* gsl_x = gsl_vector_alloc(N);
	gsl_vector* gsl_y = gsl_vector_alloc(N);

	// fill the GSL vectors
	for (int i = 0; i < N; i++){
      gsl_vector_set(gsl_x,i,vectors[0][i]);
      gsl_vector_set(gsl_y,i,vectors[1][i]);
    }

    // pointer to pointers to gsl_vectors
    gsl_vector** out = new gsl_vector*[2];
    out[0] = gsl_x;
    out[1] = gsl_y;

    // free the memory
    delete[] vectors[0];
    delete[] vectors[1];
    delete[] vectors;

    return(out);

}

gsl_vector* vector_sum(double a,gsl_vector** vectors){

	// do the vector sum:
    // y <- a*x + b*y
    gsl_vector_axpby(a,vectors[0],1.0,vectors[1]);

    // free the memory
	gsl_vector_free(vectors[0]);

	// return the result as a gsl_vector
	return(vectors[1]);

}
