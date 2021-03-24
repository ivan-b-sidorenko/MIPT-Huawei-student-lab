BLD = build
EXEC = matr.o

STD = --std=c++2a
WARNS = -Wall -Wextra -Wno-old-style-cast -Weffc++  -Wnon-virtual-dtor -Wswitch-enum -Wunused
OPT = -O0 

CC = clang++ -pthread $(STD) $(WARNS) $(OPT)

all: naive

naive: dir_bld
	$(CC) -g -o $(BLD)/$(EXEC) main.cc
 
dir_bld: 
	mkdir -p $(BLD)

clean:
	rm -rf $(BLD) $(BLD_IMP)

.PHONY: clean
