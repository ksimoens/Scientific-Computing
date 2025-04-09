output: main.o floatSum.o read_config.o randomGauss.o
	h5c++ main.o floatSum.o read_config.o randomGauss.o -o main -lgsl

read_config.o: read_config.cpp
	g++ -c read_config.cpp

randomGauss.o: randomGauss.cpp
	g++ -c randomGauss.cpp

floatSum.o: floatSum.cpp
	g++ -c floatSum.cpp -lgsl

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o main