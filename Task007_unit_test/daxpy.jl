using Test # library for unit testing

# structure that holds the parameters
struct params

	N::Int64 	# length of the vectors
	a::Float64	# coefficient of the sum
	X::Float64	# value of the x vector
	Y::Float64	# value of the y vector

end

# function that reads the parameters from the configuration file
function read_params(path::String)

	param_file = readlines(path)

	N = 0
	a = 0.0
	X = 0.0
	Y = 0.0
	
	for i in 1:length(param_file)

		if startswith(param_file[i],"N")
			N = parse(Int64,split(param_file[i]," ")[2])
		elseif startswith(param_file[i],"a")
			a = parse(Float64,split(param_file[i]," ")[2])
		elseif startswith(param_file[i],"X")
			X = parse(Float64,split(param_file[i]," ")[2])
		elseif startswith(param_file[i],"Y")
			Y = parse(Float64,split(param_file[i]," ")[2])
		end

	end

	myparams = params(N,a,X,Y)

	return(myparams)

end

# create the vectors to be used in the sum
function create_vectors(N::Int64,X::Float64,Y::Float64)

	return((ones(Float64,N)*X,ones(Float64,N)*Y))

end

# perform the vector sum (broadcasting)
function vector_sum(a::Float64,vectors::Tuple{Array{Float64},Array{Float64}})

	return(a*vectors[1] .+ vectors[2])

end

# test the parameter reading
function test_read()

	# test whether all parameters are 1 as in the test file
	myparams = read_params("config_test")
	@test myparams.N == 1
	@test myparams.a == 1.0
	@test myparams.X == 1.0
	@test myparams.Y == 1.0

end

# test whether the vectors are created correctly
function test_vectors()

	# test the length of the vectors
	@test length(create_vectors(10,1.0,1.0)[1]) == 10
	@test length(create_vectors(10,1.0,1.0)[2]) == 10
	# test the values of the vectors
	@test create_vectors(10,1.0,1.0)[1] == ones(Float64,10)
	@test create_vectors(10,1.0,1.0)[2] == ones(Float64,10)

end

# test the vector sum with some combinations
function test_vector_sum()

	#test the length of the resulting vector
	@test length(vector_sum(1.0,create_vectors(10,1.0,1.0))) == 10

	# test the values of the resulting vector
	# a*x + y = 1.0*1.0 + 1.0 = 2.0
	@test vector_sum(1.0,create_vectors(10,1.0,1.0)) == ones(Float64,10)*2.0
	# a*x + y = 0.0*1.0 + 1.0 = 1.0
	@test vector_sum(0.0,create_vectors(10,1.0,1.0)) == ones(Float64,10)
	# a*x + y = 1.0*0.0 + 0.0 = 0.0
	@test vector_sum(1.0,create_vectors(10,0.0,0.0)) == zeros(Float64,10)
	# a*x + y = 0.0*1.0 + 0.0 = 0.0
	@test vector_sum(0.0,create_vectors(10,1.0,0.0)) == zeros(Float64,10)

end

function main()

	# unit tests
	# test the parameter reading
	test_read()
	# test the vector creation
	test_vectors()
	# test the vector sum
	test_vector_sum()

	# read the parameters from the configuration file
	myparams = read_params("config")
	# perform the sum and write the result as output
	println("the result of the vector sum is: "*
				string(vector_sum(myparams.a,
					create_vectors(myparams.N,myparams.X,myparams.Y))[1]))


end

main()