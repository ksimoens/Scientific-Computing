# Parallel Computing with GPU

I have to be honest here; I am a bit disappointed about this part. It was my intention to play around with GPU parallel programming, but the exercise turned into a lot of frustration while attempting to simply have access to a working GPU. These attempts have been in vain. This report therefore contains the narrative of failed attempts.   
  
The full code of the implementation of the daxpy vector sum on the GPU can be found on [GitHub](https://github.com/ksimoens/Scientific-Computing/tree/master/Task010_parallel_GPU).

## CUDA

Without access to a GPU with properly installed CUDA drivers, the only way of trying out GPU programming is Google Colab. I never use Google Colab, R Markdown, Jupyter Notebook or similar tools as I find this type of editors 'programming with training wheels'.

### Google Colab

This implementation of the daxpy vector sum can be found in the **daxpy.ipynb** notebook in the **CUDA** directory.  
In order to use the NVIDIA GPU on Google Colab, you have to first switch the *Runtime* to GPU: *Runtime > Change runtime type > Hardware accelerator > T4 GPU > Save*. The CUDA driver should now be available: 

```
$ !nvcc -- version
$ 	nvcc: NVIDIA (R) Cuda compiler driver
$	Copyright (c) 2005-2024 NVIDIA Corporation
$	Built on Thu_Jun__6_02:18:23_PDT_2024
$	Cuda compilation tools, release 12.5, V12.5.82
$	Build cuda_12.5.r12.5/compiler.34385749_0
```

as well as the GPU specifications:

```
Fri Jun  6 11:20:05 2025       
+-----------------------------------------------------------------------------------------+
| NVIDIA-SMI 550.54.15              Driver Version: 550.54.15      CUDA Version: 12.4     |
|-----------------------------------------+------------------------+----------------------+
| GPU  Name                 Persistence-M | Bus-Id          Disp.A | Volatile Uncorr. ECC |
| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |
|                                         |                        |               MIG M. |
|=========================================+========================+======================|
|   0  Tesla T4                       Off |   00000000:00:04.0 Off |                    0 |
| N/A   40C    P8              9W /   70W |       0MiB /  15360MiB |      0%      Default |
|                                         |                        |                  N/A |
+-----------------------------------------+------------------------+----------------------+
                                                                                         
+-----------------------------------------------------------------------------------------+
| Processes:                                                                              |
|  GPU   GI   CI        PID   Type   Process name                              GPU Memory |
|        ID   ID                                                               Usage      |
|=========================================================================================|
|  No running processes found                                                             |
+-----------------------------------------------------------------------------------------+
```

In order to use `nvcc` in the Jupyter cells, you need the Jupyter extension:

```
$ !pip install nvcc4jupyter
$ 	Collecting nvcc4jupyter
$	Downloading nvcc4jupyter-1.2.1-py3-none-any.whl.metadata (5.1 kB)
$	Downloading nvcc4jupyter-1.2.1-py3-none-any.whl (10 kB)
$	Installing collected packages: nvcc4jupyter
$	Successfully installed nvcc4jupyter-1.2.1
```

which can then be loaded:

```
$ %load_ext nvcc4jupyter
$	Detected platform "Colab". Running its setup...
$	Source files will be saved in "/tmp/tmp6d210jjm".
```

Now C++ scripts with CUDA code can simply be executed from code cells in Jupyter when they are introduced by the `%%cuda` extension  
  
For the rest, the code cell performing the daxpy vector sum with CUDA is just a normal C++ programme that makes use of the CUDA language.  

1. The host vectors are allocated and filled.  
2. The device vectors are allocated using the `cudaMalloc()` function.  
3. The data from the host vectors is transferred to the device vectors using the `cudaMemcpy()` function.
4. The defined kernel is executed. The kernel gets the vector index from `threadIdx.x` and performs the sum for each element in the vectors. The structure is therefore onedimensional.
5. The calculated data from the device vector is copied back to the host vector with the `cudaMemcpy` function.
6. Some output is printed to the terminal and the host and device memory is cleared.

The code runs perfectly, but the resulting vector sum is consistently 0.  
First, the data transfer is tested by filling the host vectors, copying them to the device, resetting them to 0 and copying them back from the device. The resulting vector gives the correct value. Consequently, the data is correctly transferred to and from the device.  
To test the actual GPU calculations, a `gpuAssert` wrapper can be used. This function is added to the script before the main function. After the execution of the kernel, the `cudaPeekAtLastError()` function can return the execution failures on the device. Indeed, an error is returned:

```
$ GPUassert: the provided PTX was compiled with an unsupported toolchain.
```

This error simply indicates that there is a mismatch between the PTX (Parallel Thread Execution) code and the CUDA toolkit or driver versions. The PTX code, which is a kind of intermediate language for CUDA kernels, was compiled with a toolchain (like the nvcc compiler) that is not compatible with the CUDA toolkit and driver installed on your system. This can happen if the PTX code was generated using a CUDA toolkit that is either older or newer than the one you have on your machine.  
Indeed, as seen before, the `nvcc` compiler runs on CUDA version 12.5, while the GPU itself runs on version 12.4.  
As attempts to reinstall the correct version of the compiler failed, this exercise ends here.

## Open Accelerator (OpenAcc) 

The OpenAcc implementation of the daxpy vector sum can be found in the **daxpy.c** script in the **OpenAcc** directory. To run the script, first compile withe `make` and then run the executable `./daxpy`. 
The compiler is made aware of OpenAcc by adding the `-fopenacc` flag:

```
$ gcc -g -O3 -fopenacc main.c -o main
```

OpenAcc is implemented by adding decorators to the code in the same way as for OpenMP:

```
#pragma acc parallel loop copyin(vecX[0:N], vecY[0:N]) copyout(vecZ[N])
```

The `copyin` and `copyout` directives explicitly indicate that the X and Y vectors have to copied from the host to the device and the Z vector from the device to the host.  
The programme runs fine on all platforms, including my personal laptop, which has no GPU. OpenAcc is able to use the CPU as well when there is no accelerator device available. It is therefore important to test whether OpenAcc really uses the GPU. To do this, a testing loop is written:

```
int NUM = 1000;
int* buf;
buf = (int*) malloc(NUM*sizeof(int));
for(int i=0;i<NUM;i++){
	buf[i] = 0;
}

#pragma acc parallel create(buf[NUM])
for(int i=0;i<NUM;i++){
	buf[i] = 1;
}

if (buf[0] == 1) {
	printf("Running on CPU\n");
} else {
	printf("Running on GPU\n");
}
```

The buffer `buf` is created on the host and filled with zeros. Next, the buffer is transferred to the device using the `create()` directive. The buffer is never returned to the host.  
Consequently, if the programme is run on the GPU device, the buffer values will be changed to 1 on the device, but the values of the buffer on the host remain unchanged. If the buffer values are still 0, the GPU was used to perform the operations. In contrast, if the programme is run on the CPU, the values of the buffer on the host will be directly changed to 1.

### CloudVeneto

To check whether our research group has access to a GPU device, the following command can be used:

```
$ sudo lshw -C display
$  *-display                 
$       description: VGA compatible controller
$       product: Virtio GPU
$       vendor: Red Hat, Inc.
$       physical id: 2
$       bus info: pci@0000:00:02.0
$       logical name: /dev/fb0
$       version: 01
$       width: 64 bits
$       clock: 33MHz
$       capabilities: msix vga_controller bus_master cap_list rom fb
$       configuration: depth=32 driver=virtio-pci latency=0 mode=1280x800 visual=truecolor xres=1280 yres=800
$       resources: iomemory:38000-37fff irq:10 memory:fe000000-fe7fffff memory:380000000000-380000003fff memory:febd0000-febd0fff memory:c0000-dffff

```

I have tried to find some information about this [VirtIO GPU](https://www.qemu.org/docs/master/system/devices/virtio-gpu.html), but I do not fully understand what type of device this is and whether this can be used for computation. Our supervisors have told us that GPUs are available, but nobody has ever tried to use them so far.  
  
Compiling the script on the CloudVeneto machine results in the following error:

```
$ to1: error: ‘-fcf-protection=full’ is not supported for this target
$ mkoffload: fatal error: x86_64-linux-gnu-accel-nvptx-none-gcc-9 returned 1 exit status
$ compilation terminated.
```

After adding the flag `-fcf-protection=none` to the compiler, everything compiles nicely.  
The output of the buffer test loop is clear:

```
$ Running on CPU
```

This is again a failed attempt to run a script on a GPU. Given that I do not know anything about this type of GPU, it is impossible to find out why this test fails.

### Google Colab

A last attempt is made by running the OpenAcc script on Google Colab. First the **daxpy.c** script and the **Makefile** have to be uploaded to the Google Colab directory (Google Cloud). Next, the code can be easily compiled and executed by running `!make` and `!./daxpy` in the code cells respectively.  
Yet again the output of the programme is:

```
$ Running on CPU
```

As Google Colab surely has access to an NVIDIA GPU, the problem lies in the fact that the compiler and driver do not seem to recognise this device.  
An [online tutorial](https://enccs.github.io/OpenACC-CUDA-beginners/setup/) provides code to run OpenAcc scripts on the Google Colab GPU that is almost identical to my code. Additionally, adding my test loop to their code, clearly shows that also their programme is run on the CPU.  
Trying out different flags and `pragma` statements did not work, and I have given up. I do not understand why it does not work.

## Conclusion

Compared to running code in parallel on the CPU, using GPU seems to be a lot harder. If drivers and software are not already properly installed, you need a lot of expertise to do it yourself. I find the online information to be very limited and very hard to understand as compared to what is available on MPI or OpenMP. The fact that there is much less homogeneity in the software-hardware interface makes it very hard to debug.  
I do not understand what type of GPU is available on the CloudVeneto machine (VirtIO), let alone what to do to install the proper drivers. I understand why CUDA does not work in Google Colab (incompatible CUDA versions) but have no knowledge on how to reinstall CUDA properly. I understand even less why the OpenAcc script is not run on the Google Colab GPU.  
At home, I have an old gaming laptop with an NVIDIA GPU. During this Summer, I will try to install the necessary drivers and to run some code on this GPU.