# Data Storage with HDF5

## Install HDF5

In order to use the HDF5 C-library, you need to install the HDF5 tools.  
On Ubuntu, first install the following tools:

```
$ sudo apt-get install libhdf5-dev hdf5-tools
```  

Next locate the `hdf5.h` file in your file system:  

```
$ find /usr -iname "*hdf5.h*"
```

This will return `/path/to/hdf5/directory/hdf5.h`.  
To be able to use the `hdf5.h` file anywhere, export the HDF5 path: 

```
$ export CPATH="/path/to/hdf5/directory/"
```

Now you can use `#include "hdf5.h"` in the header of any C/C++ file on your computer.  
To compile C++ scripts that use the `hdf5.h` library, compile the script with the `h5c++` compiler instead of with the ordinary `g++` one.

## Install GSL

To install the GSL library on Ubuntu, use:

```
$ sudo apt-get install libgsl-dev
```

You can then include the library in any C++ programme by `#include <gsl/your_gsl_app.h>`.  
In order to let the compiler know about the existence of GSL, the `-lgsl` flag has to be used in the compile string.

## Run the script

The code can be found on [GitHub](https://github.com/ksimoens/Scientific-Computing/tree/master/Task003_data_storage) under **Task_003_data_storage**.  

In order to run the vector-sum script with its input and output, execute the bash script `run.sh`.  
You might have to make it executable via

```
$ chmod u+x run.sh 
```

The script first sets the path for the `h5c++` compiler, then compiles the scripts with the `make` command using the Makefile, executes the programme and finally cleans the directory.  
  
Parameters are included in two ways.  
The size of the vectors and the values of the X and Y vectors are provided to the programme via the command line. They can be changed in the `run.sh` script.  
The value of the *a* coeffecient and the name of the output HDF5 file are provided in the **config** file and can be changed there. The output is stored in the hdf5 file with the name '\[your\_output\_name\]\_N\[length\_of\_vector\].h5'. An example is provided in '**output_N1000.h5**'.

## Reflections

The learning curve for working with 'HDF5' in C/C++ is very steep. Reading and writing such file formats is exceedingly easy in Python or R, but in C++ you have to do everything yourself.  
The current state of the programme is a mess. Code is reproduced many times just to read and write the HDF5 file. Attempts have been made to wrap the reading and writing in a class, but these attempts have failed.  
However, the HDF5 and NetCDF file formats will be indispensable for my work with oceanographic data and very large simulation output files.