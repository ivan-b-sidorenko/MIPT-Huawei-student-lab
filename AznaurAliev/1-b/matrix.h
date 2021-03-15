#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "assert.h" 
#include <immintrin.h>


class matrix 
{
public:
	int num_col;
        int num_str;
public:
	matrix(int col , int str);
	matrix(int col, int str, bool fl);
	matrix(const matrix& rhs);

	~matrix();

	bool operator==(const matrix& rhs);
	int* operator[](const int rhs) const {return ptr[rhs];};

	friend std::ostream& operator<<(std::ostream& out , const matrix& rhs);

private:
	int** ptr;
};

