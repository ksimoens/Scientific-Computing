# Unit Testing

## C++

Multiple frameworks for Unit Testing (UT) exist for C++. These frameworks greatly simplify and automatise UT. However, most if not all frameworks are integrated in CMake. See for example [CTest](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Testing%20With%20CMake%20and%20CTest.html) and [Catch2](https://github.com/catchorg/Catch2).  
For the moment, CMake is not one of my priorities. Therefore, I have written my own UT functions.  
  
The programme is cut in an unnatural number of pieces that are tested individually.  
First the reading of parameters from the configuration file is tested.  
Second the vector sum itself is tested, divided into the creation of vectors and the actual vector sum. These tests are defined in **test.h** and **test.cpp**.  
When a test fails, a message is printed with the function and line number of the test that failed. This error message is defined in **globParams.h**.

## Julia

As always, doing the same things in Julia is much easier than doing them in C++. The standard testing library for Julia is [Test](https://docs.julialang.org/en/v1/stdlib/Test/) and is straighforward to use. Exactly the same steps (with the exception of the GSL vector creation) as in the C++ code are tested in Julia (**daxpy.jl**).  
When a test fails, the excecution is aborted reporting which test failed:

```
Test Failed at /PATH_TO_SCRIPT/daxpy.jl:62
  Expression: myparams.N == 2
   Evaluated: 1 == 2

ERROR: LoadError: There was an error during testing
```

## Conclusion

While I have never performed rigourous UT, I always test individual functions when I implement them. That being said, even if all functions do what they are meant to do individually, then still the whole programme can show unintended behaviour or the whole is not equal to the sum of the parts.  
  
To run both the C++ and the Julia programme, execute the bash script ``$ ./run_tests.sh``. Change the parameters in the *config* file as desired.  
All the code is available on [GitHub]().