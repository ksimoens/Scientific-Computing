# main script that executes the full programme

# export the hdf5 path
export CPATH="/usr/include/hdf5/serial/"

# remove output files that are present
rm *.h5
rm *.png


# compile the C++ programme
# > /dev/null 2>&1: suppress command-line output
make > /dev/null 2>&1

# execute the C++ code
./main

# plot the random number result
Rscript plot.R > /dev/null 2>&1

# clean the directory
make clean > /dev/null 2>&1