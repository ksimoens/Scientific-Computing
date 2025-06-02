# Divide the Work

The implementation of a blocked daxpy vector sum is straightforward and not much can be said about it. Maybe this blocking can be interesting with the goal of parallellising the code, but most parallellisation will do this division of labour automatically.  
  
The programme can be executed by running the bash script `$ ./run.sh`. You can adjust the parameters in the *config* file as desired. The code can be found on [GitHub](https://github.com/ksimoens/Scientific-Computing/tree/master/Task008_divide).  
The programme performs the daxpy vector sums serially (as before), and in chunks and then checks whether the elements of the resulting vectors are equal for the two methods.  
Next, the sum of the elements in the resulting vector is calculated serially and in chunks. The final sum (a scalar number) is again compared between the two methods. The results of this sum are not identically the same, but the difference between the two results is smaller than machine precision and probably originates from rounding errors.  
These calculations are performed in **daxpy.cpp**. 