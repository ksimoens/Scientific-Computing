# get the necessary libraries
import scipy as sp
import numpy as np
import sys
import time

# function that recreates the C matrix from the R matrix
def create_C_from_R(R):

	# declare the new C matrix
	Cnew = np.zeros((np.shape(R)[0],np.shape(R)[0]),dtype=complex)
	# number of columns in R
	n = np.shape(R)[1]
	# number of rows in R = size of C
	N = np.shape(Cnew)[0]

	# for even sizes
	if(N % 2 == 0):
		# the mapping for N = 6
		# 0,1 -> 0,N-1
		# 0,2 -> 0,N-2

		# 1,0 -> N-1,0
		# 1,1 -> N-1,N-1
		# 1,2 -> N-1,N-2
		# 1,3 -> N-1,N-3

		# 2,0 -> N-2,0
		# 2,1 -> N-2,N-1
		# 2,2 -> N-2,N-2
		# 2,3 -> N-2,N-3

		# 3,1 -> N-3,N-1
		# 3,2 -> N-3,N-2

		# 4,0 -> N-4,0
		# 4,1 -> N-4,N-1
		# 4,2 -> N-4,N-2
		# 4,3 -> N-4,N-3

		# 5,1 -> N-5,N-1
		# 5,2 -> N-5,N-2
		########################

		# algorithm to create the elements of the C matrix
		# likely an easier solution exists
		Cnew[0,0] = R[0,0]
		Cnew[0,n-1] = R[0,n-1]
		Cnew[n-1,0] = R[n-1,0]
		Cnew[n-1,n-1] = R[n-1,n-1]
		Cnew[N-1,0] = R[N-1,0]
		Cnew[N-1,n-1] = R[N-1,n-1]
		for i in range(1,n-1):
			Cnew[0,i] = R[0,i]
			Cnew[0,N-i] = np.conjugate(R[0,i])
			Cnew[n-1,i] = R[n-1,i]
			Cnew[N-(n-1),N-i] = np.conjugate(R[n-1,i])
			Cnew[N-1,i] = R[N-1,i]
			Cnew[N-(N-1),N-i] = np.conjugate(R[N-1,i])

		for i in range(1,N-1):
			if(i != n-1):
				Cnew[i,0] = R[i,0]
				Cnew[N-i,0] = np.conjugate(R[i,0])
				for j in range(1,n):
					Cnew[i,j] = R[i,j]
					Cnew[N-i,N-j] = np.conjugate(R[i,j])

	# for odd sizes
	else:
		# the mapping for N = 5
		# 0 1 -> 0 N-1
		# 0 2 -> 0 N-2

		# 1 0 -> N-1 0
		# 1 1 -> N-1 N-1
		# 1 2 -> N-1 N-2

		# 2 0 -> N-2 0
		# 2 1 -> N-2 N-1
		# 2 2 -> N-2 N-2

		# 3 0 -> N-3 0
		# 3 1 -> N-3 N-1
		# 3 2 -> N-3 N-2

		# 4 0 -> N-4 0
		# 4 1 -> N-4 N-1
		# 4 2 -> N-4 N-2
		#####################
		Cnew[0,0] = R[0,0]

		for i in range(1,n):
			Cnew[0,i] = R[0,i]
			Cnew[0,N-i] = np.conjugate(R[0,i])

		for i in range(1,N):
			Cnew[i,0] = R[i,0]
			Cnew[N-i,0] = np.conjugate(R[i,0])
			for j in range(1,n):
				Cnew[i,j] = R[i,j]
				Cnew[N-i,N-j] = np.conjugate(R[i,j])

	return(Cnew)

# function that calculates the errors between the original matrix A
# and the one recalculated using the inverse FFT
def calc_error(A,Anew):

	# square differences
	sqq = pow(A-Anew,2)
	# ! problem if an element of A is 0
	sqq_rel = sqq/pow(A,2)

	# return the mean and median
	return(np.sqrt(np.real((np.mean(sqq),np.median(sqq),np.mean(sqq_rel),np.median(sqq_rel)))))

# main calculations
def main():

	# time the execution
	t1 = time.time()

	# read the size from the command line
	N = int(sys.argv[1])

	# create the matrix A by randomly drawing its elements from a 
	# normal distribution with mean 1 and variance 1
	A = np.random.normal(loc=1.0,scale=1.0,size=(N,N))

	# perform the complex FFT and its inverse
	C = sp.fft.fftn(A)
	Ac = sp.fft.ifftn(C)
	# perform the real FFT and its inverse
	R = sp.fft.rfftn(A)
	Ar = sp.fft.irfftn(R,s=(N,N))

	# calculate the errors between the original and the inverse FFT
	errorsC = calc_error(A,Ac)
	errorsR = calc_error(A,Ar)
	# calculate the matrix C from the matrix R
	Cnew = create_C_from_R(R)
	# the difference between the original C and the CfromR
	Cdev = np.abs(np.sum(Cnew-C))

	# print all the results to the command line
	print("the NumPy machine precision is %e" % np.finfo(float).eps)
	print("")
	print("the mean absolute error of the complex FFT is %e" % (errorsC[0]))
	print("the median absolute error of the complex FFT is %e" % (errorsC[1]))
	print("the mean relative error of the complex FFT is %e" % (errorsC[2]))
	print("the median relative error of the complex FFT is %e" % (errorsC[3]))
	print("")
	print("the mean absolute error of the real FFT is %e" % (errorsR[0]))
	print("the median absolute error of the real FFT is %e" % (errorsR[1]))
	print("the mean relative error of the real FFT is %e" % (errorsR[2]))
	print("the median relative error of the real FFT is %e" % (errorsR[3]))
	print("")
	print("the (0,0) element of the FFT matrix is %.16f" % np.real(C[0,0]))
	print("the sum of all elements in matrix A is %.16f" % np.sum(A))
	print("")
	print("the difference between matrix C and CfromR is %.16f" % Cdev)
	print("")
	print("the elapsed wall time is %.3f seconds" % (time.time()-t1))

main()
