# Fast Fourier Transform

## Programme

This Python script performs some calculations using the Fast Fourier Transform algorithm (FFT).  
The [scipy.fft](https://docs.scipy.org/doc/scipy/tutorial/fft.html) library is used to perform the FFT and the [NumPy](https://numpy.org/) library handles some calculations with complex numbers.  
  
The script performs the following calculations. First, a matrix is created with size **N**, read from the command line. The elements of this matrix are drawn from a normal distribution with mean and variance 1.  
Secondly, the FFT of this matrix is calculated in two ways: once by using the complex-to-complex FFT (cFFT) and once by using the real-to-complex FFT (rFFT).  
Thirdly, from both FFTs, the original matrix is recalculated by using the respective inverse FFT. The mean and median absolute and relative differences with the original matrix are calculated.  
Lastly, the output from the rFFT is used to calculate the output from the cFFT.  
  
To run the script, execute the bash file `$ ./run_calculation.sh`, in which you can change the size of the matrix **N**.  
The code can be found on [GitHub](https://github.com/ksimoens/Scientific-Computing/tree/master/Task006_FFT).

## Machine precision

The mean and median absolute and relative differences between the original matrix and the one recalculated using the inverse FFT are hitting machine precision. The SciPy library apparently ensures the maximum possible precision in these FFT calculations. The size of the matrix does not affect these final errors.  
  
*Note*: in the relative errors, the difference between the elements is divided by the element of the original matrix A. Consequently, this element cannot be zero.

## Average value

The first element in the matrices obtained from the FFT is equal to the sum of all the elements in the matrix:

$$
	C[0,0] = \sum\limits_{i=0}^{N-1}\sum\limits_{j=0}^{N-1} A[i,j]
$$  
  
This first element is commonly known as the "DC term" as it represents the average of the full AC signal. In general, this term is eliminated by performing the FFT on $A-\langle A\rangle$.

## cFFT from the rFFT

As the FFT of a real-valued matrix is Hermitian-symmetric, the *scipy.fft.rfft* function only returns the upper half of the matrix. The *scipy.fft.fft* function, in contrast, returns the full FFT matrix as it treats the input matrix as having complex elements.  
It is therefore possible to reconstruct the cFFT result from the rFFT result by taking the complex conjugate of the latter's elements.  
The ordering of the elements by the scipy.fft library seems very chaotic, but a general algorithm has been written. This algorithm reconstructs the cFFT result from the rFFT result for any size of the original matrix (odd and even). This reconstruction is identical to the actual cFFT output.   