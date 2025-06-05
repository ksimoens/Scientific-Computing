output: main.o read_config.o
	mpic++ main.o read_config.o -o main 

main.o: main.cpp
	mpic++ -c main.cpp

read_config.o: read_config.cpp
	mpic++ -c read_config.cpp

clean:
	rm *.o main