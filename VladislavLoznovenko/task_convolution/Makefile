CXX = clang++
CXXFLAGS = -c -Wall -O0 -std=c++14 -MD -mavx -mfma

.PHONY: clean

all: main

main: main.o Chanel_h.o Matrix_h.o Tensor4.o Conv_naiv.o
	 $(CXX) Tensor4.o Chanel_h.o Matrix_h.o Conv_naiv.o main.o -o $@

Matrix_h.o: Matrix_h/Matrix_h.cpp
	 $(CXX) Matrix_h/Matrix_h.cpp $(CXXFLAGS)

Chanel_h.o: Chanel_h/Chanel_h.cpp
	 $(CXX) Chanel_h/Chanel_h.cpp $(CXXFLAGS)

Tensor4.o: Tensor4_h/Tensor4.cpp
	 $(CXX) Tensor4_h/Tensor4.cpp $(CXXFLAGS)

Conv_naiv.o: Convolutions_h/Conv_naiv.cpp
	 $(CXX) Convolutions_h/Conv_naiv.cpp $(CXXFLAGS)

main.o: main.cpp
	 $(CXX) main.cpp $(CXXFLAGS)

clean:
	rm -rf *.o *.d main Matrix Chanel Tensor4

-include *.d