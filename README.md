# Scientific Computing for Physics Students
PhD course in the PhD programme in Physics of the Università degli Studi di Padova.  
  
During the course, 10 tasks had to be completed as personal work. This GitHub directory contains these tasks ordered in subdirectories. Each task contains its own instructions and README file, which indicate how the task was performed or how to run example code. This code has been tested on a personal laptop, but its functioning cannot be guaranteed on other systems.  
  
I had some personal goals:

1. Try out things that I had not done before;
2. Use as many languages as I could. For example, running simulations in Julia for efficiency but making plots in R's ggplot.
3. Advance my knowledge of the C/C++ compiled languages, including compilation Makefiles and parallel programming.
4. Find some fun in frustration...

## [TASK 001](https://github.com/ksimoens/Scientific-Computing/tree/master/Task001_Docker) - AlmaLinux 9 via Docker

This exercise contains a manual on how to install an AlmaLinux 9 image by using Docker.

## [TASK 002](https://github.com/ksimoens/Scientific-Computing/tree/master/Task002_Hello_World) - Vector Sum and Matrix Multiplication

This exercise contains the C++ and Julia source code to perform a daxpy vector sum and matrix multiplication.

## [TASK 003](https://github.com/ksimoens/Scientific-Computing/tree/master/Task003_data_storage) - HDF5 Data Storage and GSL Libraries

This exercise contains the C++ programme that performs a daxpy vector sum with reading and writing data in the HDF5 format and by using GSL libraries.

## [TASK 004](https://github.com/ksimoens/Scientific-Computing/tree/master/Task004_integration) - Numerical Integration and Errors

In this exercise, an integral is solved numerically by using discrete algorithms. The resulting errors are investigated by comparing the numerical result to the known analytical result. The exercise is done in both C++ and Julia and some plots are made in R.

## [TASK 005](https://github.com/ksimoens/Scientific-Computing/tree/master/Task005_float_random) - Floating-Point Numbers and Random Numbers

This exercise explores the nature of floating-point numbers in the summation of small and large numbers. Additionally, Gaussian random numbers are used in C++ to fill vectors.

## [TASK 006](https://github.com/ksimoens/Scientific-Computing/tree/master/Task006_FFT) - Fast Fourier Transform

This task asked to perform the Fast Fourier Transform algorithm on a random matrix. The code is written in Python.

## [TASK 007](https://github.com/ksimoens/Scientific-Computing/tree/master/Task007_unit_test) - Unit Testing

In this exercise the daxpy vector sum programme is used to introduce the concept of Unit Testing. In the C++ script, manual tests are implemented while in Julia the `Test` library is used.

## [TASK 008](https://github.com/ksimoens/Scientific-Computing/tree/master/Task008_divide) - Divide Operations

As a preparation for parallellisation, the daxpy vector sum is performed in blocks. The final result is compared to the simple serial sum by the C++ script.

## [TASK 009](https://github.com/ksimoens/Scientific-Computing/tree/master/Task009_parallel_CPU) - Parallel Computation CPU

These C++ scripts parallellise the daxpy vector sum using the Message Passing Interface (MPI) and Open MultiProcessing (OpenMP) tools. Benchmarks are summarised in a few plots generated in R.

## [TASK 010](https://github.com/ksimoens/Scientific-Computing/tree/master/Task010_parallel_GPU) - Parallel Computation GPU

This directory contains a summary of the failed attempts to run C++ and C code on a Graphics Processing Unit (GPU) by using Compute Unified Device Architecture (CUDA) and Open Accelorator (OpenAcc).

---

Contact: Kobe Simoens (kobe.simoens@studenti.unipd.it)
