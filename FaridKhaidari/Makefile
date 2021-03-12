BLD = .m_build
MST = mul_speed_tst

CC = g++ --std=c++2a -Wall -Wextra -O0

all: tst

start: bld_dir

tst: bld_dir mul_speed.o
	$(CC) -o speed.exe $(BLD)/mul_speed.o

bld_dir:
	mkdir -p $(BLD)

mul_speed.o: $(MST)/mul_speed.cc
	$(CC) -c -o $(BLD)/mul_speed.o $(MST)/mul_speed.cc

clean:
	rm -rf $(BLD)
	rm speed.exe || true

.PHONY: clean