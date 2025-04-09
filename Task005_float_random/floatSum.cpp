#include "floatSum.h"

// calculate the sum with a for loop
double loopSum(int order){

	// declare the array
	double vec[4];
	if(order == 0){
		vec[0] = 1.0; 
		vec[1] = 1e-16;
		vec[2] = -1e-16;
		vec[3] = -0.5;
	} else{
		vec[0] = 1e-16; 
		vec[1] = -1e-16;
		vec[2] = 1.0;
		vec[3] = -0.5;
	}

	// initialise the sum
	double sum = 0.0;
	// do the sum
	for(int i=0; i < 4; i++){
		sum += vec[i];
	}

	return(sum);

}

// calculate the sum via the GSL vector library
double gslSum(){

	// allocate the memory for the GSL vector
	gsl_vector* vec = gsl_vector_alloc(4);
	// fill the vector
	gsl_vector_set(vec,0,1.0);
	gsl_vector_set(vec,1,1e-16);
	gsl_vector_set(vec,2,-1e-16);
	gsl_vector_set(vec,3,-0.5);

	// calculate the sum
	double sum = gsl_vector_sum(vec);

	// free the memory
	gsl_vector_free(vec);

	return(sum);

}

// calculate the sum using the Kahan algorithm
double kahanSum(){

	// declare the array
	double vec[4]; 
	vec[0] = 1.0; 
	vec[1] = 1e-16;
	vec[2] = -1e-16;
	vec[3] = -0.5;

	// variable to hold the final result for the sum
	double sum = 0.0;
	// compensation variable
	double comp = 0.0;

	for(int i = 0; i < 4; i++){

		// for i = 0, this operation does nothing
		// for i > 0, this operation adds the low-order digits of y 
		// from the previous iteration to the calculation of sum in the 
		// next iteration => guard digits
		double y = vec[i] - comp;
		// if y << sum; the low-order digits of y will be lost
		double t = sum + y;
		// (t-sum) cancels the high-order part of sum
		// subtracting y then adds 
		// the (negative) low-order digits of y back to comp
		comp = (t-sum) - y;

		sum = t;

	}

	return(sum);

}