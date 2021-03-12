CXX = clang++
CXXFLAGS = -c -Wall -mavx2 -mavx -O0 -std=c++14 -MD

.PHONY: clean

all: main

main: main.o Matrix.o
	 $(CXX) Matrix.o main.o -o $@

Matrix.o: Matrix_lib/Matrix.cpp
	 $(CXX) Matrix_lib/Matrix.cpp $(CXXFLAGS)

main.o: main.cpp
	 $(CXX) main.cpp $(CXXFLAGS)

clean:
	rm -rf *.o *.d main Matrix

-include *.d