CXX = clang++
CXXFLAGS = -c -Wall -mavx2 -mavx -O0 -std=c++14

all: main

main: main.o Matrix.o
	@ $(CXX) Matrix.o main.o -o test

Matrix.o: Matrix.cpp
	@ $(CXX) Matrix.cpp $(CXXFLAGS)

main.o: main.cpp
	@ $(CXX) main.cpp $(CXXFLAGS)

clean:
	rm -rf *.o main Matrix