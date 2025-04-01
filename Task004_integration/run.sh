# main script that executes the full programme

# export the hdf5 path
export CPATH="/usr/include/hdf5/serial/"

# timers
start=`date +"%s.%3N"`

# if the summary file exists, delete it
if [ -f ./summary.csv ]; then
    rm summary.csv
fi

# if the summary plots exist, delete them
if [ -f ./summary_plots.pdf ]; then
    rm summary_plots.pdf
fi

# if the output directory exists, delete its content
if test -d ./output; then
	if [ -z "$( ls -A './output/' )" ]; then
	   rm output/*
	fi
fi

# compile the C++ programme
# > /dev/null 2>&1: suppress command-line output
make > /dev/null 2>&1

# run the C++ programme
# for each number of points from 2^3 to 2^20
for expon in $(seq 3 1 24); do
	N=$((2**$expon))
	# change the number of points in the config file
	CONFIG_FILE="config"
	sed -i "1s/N .*/N $N/" "$CONFIG_FILE"
	# execute the C++ code
	./main > /dev/null 2>&1
done

# clean the directory
make clean > /dev/null 2>&1

end=`date +"%s.%3N"`
runtime_cpp=$(echo "$end - $start" | bc)
echo "C++ integration programme time: ${runtime_cpp} seconds"

start=`date +"%s.%3N"`
# run the Julia script
julia integrate.jl > /dev/null 2>&1
end=`date +"%s.%3N"`
runtime_julia=$(echo "$end - $start" | bc)
echo "Julia integration runtime: ${runtime_julia} seconds"

start=`date +"%s.%3N"`
# make the plots in R
Rscript plot.R > /dev/null 2>&1
end=`date +"%s.%3N"`
runtime_R=$(echo "$end - $start" | bc)
echo "R plotting runtime: ${runtime_R} seconds"

runtime_tot=$(echo "$runtime_cpp + $runtime_julia + $runtime_R" | bc)
echo "total runtime: ${runtime_tot} seconds"