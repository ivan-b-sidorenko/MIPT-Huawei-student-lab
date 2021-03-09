BLD = build
FILES = main.cc
EXEC = 1a
STD = --std=c++2a
WARNS = -Wall -Wextra -Wno-old-style-cast -Weffc++ -Waggressive-loop-optimizations -Wnon-virtual-dtor -Wswitch-enum -Wunused -Wuseless-cast
OPT = -O0

CC = g++ $(STD) $(WARNS) $(OPT) -o $(BLD)/$(EXEC) $(FILES)

all: release

release: dir_bld
	$(CC) -D NDEBUG
 
dir_bld: 
	mkdir -p $(BLD)

debug: dir_bld
	$(CC) -g 

clean:
	rm -rf build

.PHONY: clean
