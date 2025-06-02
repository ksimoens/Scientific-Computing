output: main.o read_config.o daxpy.o test.o
	g++ main.o read_config.o daxpy.o test.o -o main -lgsl

daxpy.o: daxpy.cpp
	g++ -c daxpy.cpp -lgsl

read_config.o: read_config.cpp
	g++ -c read_config.cpp

test.o: test.cpp
	g++ -c test.cpp

main.o: main.cpp
	g++ -c main.cpp -lgsl

clean:
	rm *.o main