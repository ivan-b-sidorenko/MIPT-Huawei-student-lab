BLD = .m_build

WRN = -Wall -Wextra -Wunused -Wno-old-style-cast -Waggressive-loop-optimizations -Wnon-virtual-dtor -Wuseless-cast
CC = g++ --std=c++2a -O0 -g

all: tst

start: bld_dir

tst: bld_dir main.o
	$(CC) -pthread -o speed.exe $(BLD)/main.o

bld_dir:
	mkdir -p $(BLD)

main.o: main.cc
	$(CC) -c -pthread -o $(BLD)/main.o main.cc

clean:
	rm -rf $(BLD)
	rm speed.exe || true

.PHONY: clean