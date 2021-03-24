BLD = build
FILES = main.cc

CC = g++ --std=c++2a -Wall -Wextra -Wno-old-style-cast -Weffc++ -Waggressive-loop-optimizations -Wnon-virtual-dtor -Wswitch-enum -O0 -Wunused -Wuseless-cast -o $(BLD)/1a $(FILES)

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
