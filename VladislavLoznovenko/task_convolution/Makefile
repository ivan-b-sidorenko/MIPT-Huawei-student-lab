CXX = clang++
CXXFLAGS = -c -Wall -O0 -std=c++14 -MD

.PHONY: clean

all: main

main: main.o Chanel_h.o Matrix_h.o Tensor4.o 
	 $(CXX) Tensor4.o Chanel_h.o Matrix_h.o main.o -o $@

Matrix_h.o: Matrix_h/Matrix_h.cpp
	 $(CXX) Matrix_h/Matrix_h.cpp $(CXXFLAGS)

Chanel_h.o: Chanel_h/Chanel_h.cpp
	 $(CXX) Chanel_h/Chanel_h.cpp $(CXXFLAGS)

Tensor4.o: Tensor4_h/Tensor4.cpp
	 $(CXX) Tensor4_h/Tensor4.cpp $(CXXFLAGS)

main.o: main.cpp
	 $(CXX) main.cpp $(CXXFLAGS)

clean:
	rm -rf *.o *.d main Matrix Chanel Tensor4

-include *.d