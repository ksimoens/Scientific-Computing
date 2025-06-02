# compile the C++ programme
make

# run the C++ tests
echo "running the C++ tests"
./main

# clean the C++ programme
make clean

# run the Julia tests
echo "running the Julia tests"
julia daxpy.jl