CXX = g++


all: main

main: main.o tensor.o
	 $(CXX) main.o tensor.o -O0 -o main -lpthread

main.o: main.cpp
	 $(CXX) -c -O0 main.cpp -lpthread

tensor.o: tensor.cpp
	 $(CXX) -c -O0 tensor.cpp -lpthread

clean:
	rm -rf *.o
