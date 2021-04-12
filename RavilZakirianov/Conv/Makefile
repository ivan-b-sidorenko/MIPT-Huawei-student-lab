CXX = g++
#CXXFLAGS = -c -Wall -O0 -std=c++14


all: main

main: main.o tensor.o
	 $(CXX) main.o tensor.o -o main -lpthread

main.o: main.cpp
	 $(CXX) -c main.cpp

tensor.o: tensor.cpp
	 $(CXX) -c tensor.cpp -lptrhead

clean:
	rm -rf *.o
