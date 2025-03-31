output: main.o read_config.o write.o calc.o
	h5c++ main.o read_config.o write.o calc.o -o main -lgsl

write.o: write.cpp
	h5c++ -c write.cpp

calc.o: calc.cpp
	h5c++ -c calc.cpp -lgsl

read_config.o: read_config.cpp
	g++ -c read_config.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm *.o main