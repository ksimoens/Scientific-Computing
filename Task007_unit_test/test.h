#ifndef TEST
#define TEST

/*

	function to test the parameter reading
	function to test the filling of the dynamical vectors
	function to test the filling of the GSL vectors
	function to test the vector sum

*/

#include "globParams.h"
#include "read_config.h"
#include "daxpy.h"

// defined in test.cpp
void test_read();
void test_vector_values();
void test_GSLvector_values();
void test_vector_sum();

#endif