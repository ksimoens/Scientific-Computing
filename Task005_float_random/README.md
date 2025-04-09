# Floating Point Operations and Random Numbers

The code can be found on [GitHub](https://github.com/ksimoens/Scientific-Computing/tree/master/Task005_float_random).

## Sum of Floating Numbers

The C++ programme calculates the sum of the following numbers: \[1.0; 1e-16; -1e-16; -0.5\].  The programme uses three methods: a simple for loop, the GSL vector sum function and the Kahan algorithm.  
The exact result is obviously 0.5, but only the Kahan sum is able to reproduce the exact result. Both the for loop and the GSL vector sum obtain the result 0.5 - 1e-16.  
The problem lies in floating-point arithmetic. The sum of two double precision floating numbers with highly different exponents (-54 versus 0) results in the loss of significant digits. The precision is not large enough to avoid that 1e-16 is taken to be exactly zero in the sum with 1.0.  
However, for some reason the sum with -1e-16 results in 1.0 - 1e-16. The subtraction of 0.5 then happens as expected.  
The Kahan algorithm explicitly catches the lost digits and manages to recuperate them in the sum.  
Changing the order of the elements in the vector, by first taking the sum of 1e-16 and -1e-16 returns the correct result as now only floating numbers with equal exponents are compared.    
  
The C++ programme uses standard libraries and the **GSL_vector** library.

## Gaussian Random Numbers

The programme first initialises the random number generator with the current time as seed. Two vectors with Gaussian random variables are created. The vector sum of the two vectors is calculated using the GSL library and finally the vectors and their sum are written to an HDF5 file.  
  
The C++ programme uses standard libraries and the **HDF5** and **GSL_vector** libraries.

## Plotting in R

The **plot.R** script plots the distribution of the vector values and stores the figure in a PNG file.  
The variance of the sum of two Gaussian variables is equal to the sum of the variances of the variables and should here be 2.0. Both the distributions and the variances are in agreement with expectations.    
  
The used R libraries are **tidyverse** for plotting and **rhdf5** for reading the HDF5 files.

## Run the full programme

To run a full programme, execute the **run.sh** script.  
This script will first run the C++ programme for the floating-point arithmetic with some output in the command line.  
The C++ programme then generates the Gaussian random vectors and their sum and stores the result in an HDF5 file.  
Finally, the distributions are plotted in the R script.