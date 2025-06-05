# read the configuration file
FILE="config"
LINE_NO_C=5   # the line number of the number of cores
# read the lines in the parameter file
i=0
while read line; do
  i=$(( i + 1 ))
  # check if on the correct line and store the line
  if [[ $i = $LINE_NO_C ]]; then
    c_line="$line"
  fi
done <"$FILE"

# get the number of cores from the line
i=0
for addr in $c_line
do
  i=$(( i + 1 ))
  if [[ $i = 2 ]]; then
     C=$addr # number of cores
  fi
done

# go to the MPI programme
cd MPI
# make the MPI executable
make
# execute the MPI programme
# adjusted for a laptop with 4 logical cores, but 2 physical cores
# change to your own specifications
if [[ $C == 4 ]]; then
	/usr/bin/time -v mpirun --use-hwthread-cpus main
else
	/usr/bin/time -v mpirun -np $C main
fi
# clean the directory
make clean

# go to the OpenMP programme
cd ../OpenMP
# make the OpenMP executable
make
# execute the OpenMP programme
/usr/bin/time -v ./main
# clean the directory
make clean

# go back to the top directory
cd ..