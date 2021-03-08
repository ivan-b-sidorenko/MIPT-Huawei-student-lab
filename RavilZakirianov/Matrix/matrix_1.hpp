#pragma once
#include <iostream>
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()
#include <chrono>
namespace linear {
    class Matrix {
        private:
            int row;
            int col;
            int** arr;
        public:
            Matrix(int _row, int _col);
            Matrix(int _row, int _col, int** _arr);
            Matrix(Matrix& A);
            bool operator*(Matrix& right);
            void print() const;
            ~Matrix();
    };

    Matrix::Matrix(int _row, int _col) {
        row = _row;
        col = _col;
        arr = new int*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new int[col];
        }
    }
    
    Matrix::Matrix(int _row, int _col, int** _arr) {
        row = _row;
        col = _col;
        arr = new int*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new int[col];
        }
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] = _arr[i][j];
            }
        }
    }

    Matrix::Matrix(Matrix& A) {
        row = A.row;
        col = A.col;
        arr = new int*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new int[col];
        }
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] = A.arr[i][j];
            }
        }
    }

    Matrix::~Matrix() {
        for(int i = 0; i < row; ++i) {
            delete[] arr[i];
        }
    }

    void Matrix::print() const {
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                std::cout << arr[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    bool Matrix::operator*(Matrix& right) {              // Matrix C = A * B;
        if ( col != right.row ) {
            std::cout << "WRONG! Incorrect matrix dimensions are set\n";
            return false;
        }
        Matrix result(row, right.col);
        for(int i = 0; i < result.row; ++i) {
            for(int j = 0; j < result.col; ++j) {
                for(int k = 0; k < col; ++k) {
                    result.arr[i][j] += (arr[i][k] * right.arr[k][j]);
                }
            }
        }
        //result.print();
        return true;
        
    }
}
