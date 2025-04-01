# Numerical Integration

The goal is to numerically integrate the integral:  

$$ 
	I = \int\limits_0^\frac{\pi}{2} dx~e^x\cos(x)
$$

and compare the results with the exact result:  

$$
	I = \frac{e^\frac{\pi}{2}-1}{2}
$$

The code can be found on [GitHub](https://github.com/ksimoens/Scientific-Computing/tree/master/Task004_integration).

## Integration in C++

The script first reads the parameters from the **config** file: *N* the number of integration points, *name* the output name, *start* the lower end of the integration interval and *end* the higher end of the integration interval.  
The C++ programme then creates the integration points and writes them to HDF5 files in the **output** directory.  
Then the files are read and the points are integrated manually with the Trapezoid rule. The function is also integrated using the **GSL** library.  
The results are stored in the **summary.csv** file with columns *N* the number of integration points, *iGSL* the integration result with **GSL** and *iTRAP* the integration result with the Trapezoid rule.  
  
The C++ programme uses standard libraries and the **HDF5** and **GSL_integration** libraries.

## Integration in Julia

The same Trapezoid rule is used to numerically integrate the integral in Julia by the **integrate.jl** script. The script reads the necessary parameters from the **config** file and performs the integration for each number of integration points *N* already in the **summary.csv** file from the C++ calculation.  
The Julia results are added to the CSV file.  
  
The Julia script load the **DataFrames** and **CSV** libraries.

## Plotting in R

The **plot.R** script makes some summary plots and stores them in **summary_plots.pdf**.  
The top-left plot shows the function realisations for the lowest and highest number of integration points in **summary.csv**.  
The top-right plot shows the absolute difference between the integration in C++ and the one in Julia in function of the number of integration points. This linear dependence is puzzling.  
The bottom-left plot shows the relative error of the C++ integration in function of the number of integration points. The error decreases quadratically with the number of points until it hits machine precision.  
The bottom-right plot shows the same relative error but now for the Julia integration. The same quadratic dependence is present, but the machine precision for Julia seems to be higher than for C++.  
  
The used R libraries are **tidyverse** and **ggpubr** for plotting and **rhdf5** for reading the HDF5 files.

## Run the full programme

To run a full programme, execute the **run.sh** script.  
This script will first run the C++ programme for *N* values from $2^3$ until $2^24$ in logarithmic jumps of 2 (22 points). Then the script will run the Julia programme doing the integration for the same *N* values. Finally, the plots are made in R.  
The different programmes are timed. The C++ and Julia programme take about the same time of nine seconds, while the plotting takes around a minute and a half. In total, the script takes around two minutes to run. Note that the R plotting can take quite a bit of RAM memory to load the HDF5 vectors. Additionally, the script generates around 500 MB of HDF5 files.