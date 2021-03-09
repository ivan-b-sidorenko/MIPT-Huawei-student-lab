BLD = build
FILES = main.cc
EXEC = 1a

FILES_IMP = main.cc
EXEC_IMP = 1a_imp
BLD_IMP = build_impr

STD = --std=c++2a
WARNS = -Wall -Wextra -Wno-old-style-cast -Weffc++ -Waggressive-loop-optimizations -Wnon-virtual-dtor -Wswitch-enum -Wunused -Wuseless-cast
OPT = -O0

CC = g++ $(STD) $(WARNS) $(OPT)

all: naive impr

naive: dir_bld
	$(CC) -g -o $(BLD)/$(EXEC) $(FILES)
 
dir_bld: 
	mkdir -p $(BLD)

dir_imp:
	mkdir -p $(BLD_IMP)

impr: dir_imp
	$(CC) -g -o $(BLD_IMP)/$(EXEC_IMP) $(FILES_IMP)

clean:
	rm -rf $(BLD) $(BLD_IMP)

.PHONY: clean
