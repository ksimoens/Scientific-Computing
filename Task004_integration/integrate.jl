#=
	Julia script to do the numerical integration
=#

# load the necassary libraries
using DataFrames, CSV

# function from the exercise 
function myfunc(x::Float64)

	return(exp(x)*cos(x))

end

# get the 'start' and 'end' parameters from the config file
function get_params()

	config = readlines("config")
	out = [0.0,0.0]
	for line in config
		param = split(line," ")[1]
		val = split(line," ")[2]
		if(param == "start")
			out[1] = parse(Float64,val)
		elseif(param == "end")
			if(val == "3.14")
				out[2] = pi
			elseif(val == "1.57")
				out[2] = pi/2
			else
				out[2] = parse(Float64,val)
			end
		end
	end

	return(out)
end

# do the Trapezoid integration
function integral_trapezoid(params::Vector{Float64},N::Int)

	# create the vectors
	x = collect(range(params[1],params[2], N))
	y = myfunc.(x) #broadcast
	dx = x[2] - x[1]
	res = 0.0
	# do the numerical integration
	for i in 1:(length(y)-1)
		res += dx*(y[i]+y[i+1])/2
	end

	return(res)

end

# main function
function main()

	# read the params
	params = get_params()
	# read the summary CSV to get the N values
	df_sum = CSV.read("summary.csv",DataFrame)
	new_res = zeros(Float64,nrow(df_sum))
	# do the numerical integration for each N value
	for i in 1:nrow(df_sum)
		new_res[i] = integral_trapezoid(params,df_sum.N[i])
	end

	# add the Julia result to the summary CSV file
	df_sum[:,"iTRAPjl"] = new_res
	CSV.write("summary.csv",df_sum)

end

main()