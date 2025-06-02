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

double* vector_sum_serial(int N,double a,double** vectors){

    // allocate memory for the result of the vector sum
    double* res = new double[N];

    for(int i=0;i<N;i++){
        res[i] = a*vectors[0][i] + vectors[1][i];
    }

    return(res);

}

double* vector_sum_chunks(int N,int k,double a,double** vectors){

    // allocate memory for the result of the vector sum
    double* res = new double[N];

    // the number of chunks
    int Nk = ceil(N/(double)k);

    // for each chunk
    for(int j=0;j<Nk;j++){
        // for each element in the chunk
        // adjust the upper limit for the last chunk
        for(int i=j*k;i<std::min((j+1)*k,N);i++){

            res[i] = a*vectors[0][i] + vectors[1][i];

        }

    }

    return(res);

}

void vector_sum_compare(int N,int k,double a,double X,double Y){

    // create the X and Y vectors
    double** vectors = create_vectors(N,X,Y);
    // do the daxpy vector sum with the two methods
    double* serial = vector_sum_serial(N,a,vectors);
    double* chunks = vector_sum_chunks(N,k,a,vectors);

    // check if each element in the vectors is equal
    bool correct = true;
    for(int i=0;i<N;i++){
        correct *= (serial[i] == chunks[i]);
    }
    IS_TRUE(correct);

    // calculate the sum of the individual vectors with the two methods
    // and compare the result
    IS_TRUE(sum_serial(N,serial) == sum_chunks(N,k,chunks));
    // check if the difference is smaller than machine precision
    IS_TRUE(abs(sum_serial(N,serial) - sum_chunks(N,k,chunks)) < 1e-16);

    // free memory
    delete[] serial;
    delete[] chunks;

}

double sum_serial(int n,double* vector){

    // simple serial sum
    double sum = 0.0;
    for(int i=0;i<n;i++){
        sum += vector[i];
    }
    return(sum);

}

double sum_chunks(int N,int k,double* vector){

    // allocate memory for the sum of the chunks
    int Nk = ceil(N/(double)k);
    double* chunks = new double[Nk];
    // dummy variable that stores the sum for each chunk
    double sum_chunk;

    for(int j=0;j<Nk;j++){
        // (re)set the sum to 0 for each new chunk
        sum_chunk = 0.0;
        // for all the elements in the chunk
        for(int i=j*k;i<std::min((j+1)*k,N);i++){

            sum_chunk += vector[i];

        }
        // store the sum of the elements in the chunk in the memory
        chunks[j] = sum_chunk;

    }

    // calculate the sum of the sum of the elements in the chunks
    double sum = sum_serial(Nk,chunks);

    // free memory
    delete[] chunks;

    return(sum);

}