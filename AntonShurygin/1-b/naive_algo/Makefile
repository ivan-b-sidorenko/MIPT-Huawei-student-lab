.PHONY: clean

CXX = g++ --std=c++2a
WRNING_OPTNS = -Wall -Wextra
RELEASE_OPTS = -O0 -mavx2 -mbmi2 -mpopcnt -mfma -mcx16 -mtune=znver1
OUTPUT_OPT = -D__MATR_OUT__
INPUT_OPT = -D__INPUT__
TIME_OPT = -D__TIMER__
CORRECT_OPT = -D__CORRECT__


all: start

clean: 
		rm -rf matrix.o matrix_extnd_output.o matrix_extnd_input.o matrix_time.o matrix_corct.o

start: main.cpp
		$(CXX) $(WRNING_OPTNS) $(RELEASE_OPTS) $(OUTPUT_OPT) $(TIME_OPT) $(CORRECT_OPT) main.cpp -o matrix.o

extnd_output: main.cpp
		$(CXX) $(WRNING_OPTNS) $(RELEASE_OPTS) $(OUTPUT_OPT) main.cpp -o matrix_extnd_output.o

extnd_input: main.cpp
		$(CXX) $(WRNING_OPTNS) $(RELEASE_OPTS) $(INPUT_OPT) main.cpp -o matrix_extnd_input.o

time: main.cpp
		$(CXX) $(WRNING_OPTNS) $(RELEASE_OPTS) $(TIME_OPT) main.cpp -o matrix_time.o

correct: main.cpp
		$(CXX) $(WRNING_OPTNS) $(RELEASE_OPTS) $(CORRECT_OPT) main.cpp -o matrix_corct.o






