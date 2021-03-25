CXX = clang++
CXXFLAGS = -c -Wall -mavx2 -mavx -O0 -std=c++14 -MD 

.PHONY: clean

all: main

main: Matrix.o matrix_mult.o main.o
	 $(CXX) Matrix.o matrix_mult.o main.o -o $@ -lOpenCL

Matrix.o: Matrix_lib/Matrix.cpp
	 $(CXX) Matrix_lib/Matrix.cpp $(CXXFLAGS)

main.o: main.cpp
	 $(CXX) main.cpp $(CXXFLAGS)

matrix_mult.o: matrix_mult.cpp
	 $(CXX) matrix_mult.cpp $(CXXFLAGS) -lOpenCL

clean:
	rm -rf *.o *.d main Matrix

-include *.d