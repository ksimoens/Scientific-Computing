#include "globParams.h"
#include "read_config.h"

//sudo apt install libopenmpi-dev
// sudo apt install openmpi-bin
// --use-hwthread-cpus

// dummy function that fills a vector with a single value 
void fill_vector(int N,double* vector,double value){

	for(int i=0;i<N;i++){
		vector[i] = value;
	}

}

// everything is written in the main function
// in order to maintain an overview over the MPI implementation
int main(int argc,char** argv){

	// initialise MPI
	MPI_Init(&argc,&argv);

	// read the parameters from the configuration file
	struct config_params myparams = read_config(CONFIGPATH);

	// initialise the necessary parameters
	int root_rank = 0;		// the rank of the root process
	double start;			// the start time
	double end;				// the end time
	double dt_serial;		// the elapsed wall time for the serial part
	double dt_parall;		// the elapsed wall time for the parallel part
	double sum_serial;		// the sum of the resulting vector serial
	double sum_parall;		// the sum of the resulting vector parallel
	int my_rank;			// the rank of the current process
	int comm_size;			// the total number of processes
	
	// get the rank and the total number of processes
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_size);

	// initialise the dynamic arrays
	double* vecX = NULL;
	double* vecY = NULL;
	double* vecZ_serial = NULL;	// contains the result of the serial sum
	double* vecZ_parall = NULL;	// contains the result of the parallel sum

	// only for the root process
	if(my_rank == root_rank){
		// fill the vectors
		vecX = new double[myparams.N];
		vecY = new double[myparams.N];
		vecZ_serial = new double[myparams.N];
		vecZ_parall = new double[myparams.N];
		fill_vector(myparams.N,vecX,myparams.X);
		fill_vector(myparams.N,vecY,myparams.Y);
		// perform the serial daxpy sum
		// get the wall time for the sum only
		start = MPI_Wtime();
		sum_serial = 0.0;
		for(int i=0;i<myparams.N;i++){
			vecZ_serial[i] = myparams.a*vecX[i] + vecY[i];
			sum_serial += vecZ_serial[i];
		}
		end = MPI_Wtime();
		dt_serial = end - start;
	}

	// make sure that every process starts from here for the parallel sum
	MPI_Barrier(MPI_COMM_WORLD);

	// start the timer
	// here also the distribution of the arrays has to be timed as this
	// reflects the overhead by MPI
	start = MPI_Wtime();
	// the length of the vectors in the local process
	int local_N = myparams.N/comm_size;
	double local_sum = 0.0;
	// initialise the dynamic arrays for the local process
	double* localX = new double[local_N];
	double* localY = new double[local_N];
	double* localZ = new double[local_N];

	// distribute the vectors over the processes
	// from GLOBAL, divide in pieces of local_N size, of type double;
	// to LOCAL, in pieces of local_N size, of type double;
	// sent from the root, to all the processes
	MPI_Scatter(vecX,local_N,MPI_DOUBLE,
				localX,local_N,MPI_DOUBLE,
				root_rank,MPI_COMM_WORLD);
	MPI_Scatter(vecY,local_N,MPI_DOUBLE,
				localY,local_N,MPI_DOUBLE,
				root_rank,MPI_COMM_WORLD);

	// perform the daxpy sum on the local pieces
	for(int i=0;i<local_N;i++){
		localZ[i] = myparams.a*localX[i] + localY[i];
		local_sum += localZ[i];
	}

	// collect the pieces back into the global array
	// from LOCAL, in pieces of local_N size, of type double;
	// to GLOBAL, in pieces of local_N size, of type double;
	// to root, from all the processes
	MPI_Gather(localZ,local_N,MPI_DOUBLE,
				vecZ_parall,local_N,MPI_DOUBLE,
				root_rank,MPI_COMM_WORLD);

	// collect the local sums into the global sum by reducing
	// reduce the LOCAL sums, into the GLOBAL sum;
	// of size 1, and of type double;
	// by taking their sum;
	// to the root from all the processes
	MPI_Reduce(&local_sum,&sum_parall,
				1,MPI_DOUBLE,
				MPI_SUM,
				root_rank,MPI_COMM_WORLD);

	// free the local memory
	delete[] localX;
	delete[] localY;
	delete[] localZ;

	// stop the timer
	end = MPI_Wtime();

	// only for root: make some summary
	if(my_rank == root_rank){

		// get the elapsed parallel wall time
		dt_parall = end - start;

		// check if the parallel result is equal to the serial result
		bool correct = true;
		for(int i=0;i<myparams.N;i++){
			correct *= (vecZ_serial[i] == vecZ_parall[i]);
		}
		IS_TRUE(correct);
		IS_TRUE(sum_serial == sum_parall);
		IS_TRUE((sum_serial - sum_parall) < 1e-16);

		// print the outcome
		std::cout << "daxpy vector sum with MPI" << std::endl;
		std::cout << "MPI run using " << comm_size << " cores" << std::endl;
		std::cout << "vectors of size " << myparams.N << std::endl;
		std::cout << "serial runtime is: " << dt_serial << " s" << std::endl;
		std::cout << "parallel runtime is: " << dt_parall << " s" << std::endl;
		std::cout << "final result of the sum is: " << vecZ_serial[0] << std::endl;
		std::cout << "the sum of the vector sum is: " << sum_serial << " for the serial process" << std::endl;
		std::cout << "the sum of the vector sum is: " << sum_parall << " for the parallel process" << std::endl;
	}

	// free the global memory
	delete[] vecX;
	delete[] vecY;
	delete[] vecZ_serial;
	delete[] vecZ_parall;

	// finish MPI	
	MPI_Finalize();

	return 0;
}