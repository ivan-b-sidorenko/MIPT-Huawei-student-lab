BLD = .m_build

CC = g++ --std=c++2a -Wall -Wextra -O0

all: tst

start: bld_dir

tst: bld_dir main.o
	$(CC) -o speed.exe $(BLD)/main.o

bld_dir:
	mkdir -p $(BLD)

main.o: main.cc
	$(CC) -c -o $(BLD)/main.o main.cc

clean:
	rm -rf $(BLD)
	rm speed.exe || true

.PHONY: clean