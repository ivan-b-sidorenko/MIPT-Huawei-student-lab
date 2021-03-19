CC = clang++
CFLAGS = -c -mavx2 -mavx -mfma -O0 -std=c++14 -MD


all: main

main: main.o matrix.o
	 $(CC) matrix.o main.o -o a.out

matrix.o: matrix.cpp
	 $(CC) matrix.cpp $(CFLAGS)

main.o: main.cpp
	 $(CC) main.cpp $(CFLAGS)

clean:
	rm -rf *.o *.d main 

-include *.d
