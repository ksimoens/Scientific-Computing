#include "test.h"

void test_read(){

	// read the parameters from the test config file
	struct config_params myparams = read_config(TESTCONFIGPATH);
	// check if all values are 1 (as in the file)
	IS_TRUE(myparams.N == 1);
	IS_TRUE(myparams.a == 1.0);
	IS_TRUE(myparams.X == 1.0);
	IS_TRUE(myparams.Y == 1.0);

}

void test_vector_values(){

	// create the dynamical vectors of size 10 with values 1.0
	double** vecs = create_vectors(10,1.0,1.0);
	
	// check if all values are equal to 1.0
	bool correct = true;
	for(int i=0;i<10;i++){
		correct *= (vecs[0][i] == 1.0);
		correct *= (vecs[1][i] == 1.0);
	}

	IS_TRUE(correct);

	delete[] vecs[0];
	delete[] vecs[1];
	delete[] vecs;

}

void test_GSLvector_values(){

	// create the dynamical vectors manually
	// size = 10
	// values = 1.0
	double* vecX = new double[10];
	double* vecY = new double[10];

	for(int i=0;i<10;i++){
		vecX[i] = 1.0;
		vecY[i] = 1.0;
	}

	double** out = new double*[2];
	out[0] = vecX;
	out[1] = vecY;

	// create and fill the GSL vectors
	gsl_vector** GSLout = create_GSLvectors(10,out);

	// check if all the values are equal to 1.0
	bool correct = true;

	for(int i=0;i<10;i++){
		correct *= (gsl_vector_get(GSLout[0],i) == 1.0);
		correct *= (gsl_vector_get(GSLout[1],i) == 1.0);
	}

	IS_TRUE(correct);

	gsl_vector_free(GSLout[0]);
	gsl_vector_free(GSLout[1]);
	delete[] GSLout;

}

void test_vector_sum(){

	// test the whole chain of functions
	// size of the vectors = 10

	// a*x + y = 1.0 * 1.0 + 1.0 = 2.0
	gsl_vector* res = vector_sum(1.0,create_GSLvectors(10,create_vectors(10,1.0,1.0)));
	bool correct = true;
	for(int i=0;i<10;i++){
		correct *= (gsl_vector_get(res,i) == 2.0);
	}
	IS_TRUE(correct);

	// a*x + y = 0.0 * 1.0 + 1.0 = 1.0
	res = vector_sum(0.0,create_GSLvectors(10,create_vectors(10,1.0,1.0)));
	correct = true;
	for(int i=0;i<10;i++){
		correct *= (gsl_vector_get(res,i) == 1.0);
	}
	IS_TRUE(correct);

	// a*x + y = 1.0 * 0.0 + 0.0 = 0.0
	res = vector_sum(1.0,create_GSLvectors(10,create_vectors(10,0.0,0.0)));
	correct = true;
	for(int i=0;i<10;i++){
		correct *= (gsl_vector_get(res,i) == 0.0);
	}
	IS_TRUE(correct);

	// a*x + y = 0.0 * 1.0 + 0.0 = 0.0
	res = vector_sum(0.0,create_GSLvectors(10,create_vectors(10,1.0,0.0)));
	correct = true;
	for(int i=0;i<10;i++){
		correct *= (gsl_vector_get(res,i) == 0.0);
	}
	IS_TRUE(correct);

	gsl_vector_free(res);

}