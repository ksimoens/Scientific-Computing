output: main.o read_config.o daxpy.o
	g++ main.o read_config.o daxpy.o -o main 

daxpy.o: daxpy.cpp
	g++ -c daxpy.cpp

read_config.o: read_config.cpp
	g++ -c read_config.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o main