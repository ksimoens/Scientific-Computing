output: main.o read_config.o
	g++ -fopenmp main.o read_config.o -o main 

main.o: main.cpp
	g++ -fopenmp -c main.cpp

read_config.o: read_config.cpp
	g++ -c read_config.cpp

clean:
	rm *.o main