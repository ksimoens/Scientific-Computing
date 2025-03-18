# load necassary modules
using Printf # for C-style printing

# function to calculate the vector sum
function vector_sum(N,a,x,y)

	vec_x = ones(N)*x
	vec_y = ones(N)*y

	return(a*vec_x .+ vec_y)

end

# function to calculate the matrix multiplication
function matrix_mult(N,a,b)

	mat_a = ones((N,N))*a
	mat_b = ones((N,N))*b

	return(mat_a * mat_b)

end	

# function to check if all elements are equal to the expected one
# multiple broadcasting: for vector sum
function check_elem(res,a,x,y)

	check = length(findall(z->(z != a*x+y),res))
	if(check==0)
		@printf("All elements are equal to %.2f.\n",a*x+y)
	else
		@printf("%i elements are not equal to %.2f.\n",check,a*x+y)
	end

end

# multiple broadcasting: for matrix multiplication
function check_elem(res,a,b)

	N = size(res)[1]

	check = length(findall(z->(abs(z/N - a*b) > 1e-10),res))
	if(check==0)
		@printf("All elements are equal to %.2f.\n",a*b*N)
	else
		@printf("%i elements are not equal to %.2f.\n",check,a*b*N)
	end

end

# main function
function main()
	
	# ------ VECTOR SUM -------
	a = 3.0
	x = 0.1
	y = 7.1
	
	@printf("Doing vector sum:\n")

	# for lengths 10, 1000000, 100000000
	for N in [10,round(Int64,1e6),round(Int64,1e8)]

		# get the time it takes to run the operation and the check
		t = time()
		check_elem(vector_sum(N,a,x,y),a,x,y)
		@printf("N = %.2e done in %f seconds.\n",N,time()-t)

	end

	# -------------------------

	# - MATRIX MULTIPLICATION -
	a = 3.0
	b = 7.1

	@printf("\nDoing matrix multiplication:\n")

	for N in [10,round(Int64,1e2),round(Int64,1e4)]

		t = time()
		check_elem(matrix_mult(N,a,b),a,b)
		@printf("N = %.2e done in %f seconds.\n",N,time()-t)

	end

	# -------------------------

end

main() # execute the main function