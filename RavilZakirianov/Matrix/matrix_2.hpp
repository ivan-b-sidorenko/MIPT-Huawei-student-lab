#pragma once
#include <iostream>
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()
#include <chrono>
#include <immintrin.h>

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
        //for(int i = 0; i < row; ++i) {
        //   for(int j = 0; j < col; ++j) {
        for(int j = 0; j < col; ++j) {
            for(int i = 7; i < row; i += 8) {
                arr[i-7][j] = A.arr[i-7][j];
                arr[i-6][j] = A.arr[i-6][j];
                arr[i-5][j] = A.arr[i-5][j];
                arr[i-4][j] = A.arr[i-4][j];
                arr[i-3][j] = A.arr[i-3][j];           //add loop unrolling
                arr[i-2][j] = A.arr[i-2][j];
                arr[i-1][j] = A.arr[i-1][j];
                arr[i][j] = A.arr[i][j];
            }
            if ( row < 8 ) {
                for(int i = 0; i < row; ++i) {
                    arr[i][j] = A.arr[i][j];
                }
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
        int M = result.row;
        int N = result.col;
        int num = 0;
        int c = 0;
        //for(int k = 0; k < col; ++k) {
        for(int j = 0; j < N; ++j) {
            int Y[col];           //столбец
            for(int q = 0; q < col; ++q) {
                Y[q] = right.arr[q][num];
            }
            num++;
            for(int i = 0; i < M; ++i) {            //ускорение перестановкой циклов, вынос переменных
                c = 0;
                for(int k = 7; k < col; k += 8) {   //loop unrolling
                    c += arr[i][k-7] * Y[k-7];
                    c += arr[i][k-6] * Y[k-6];
                    c += arr[i][k-5] * Y[k-5];
                    c += arr[i][k-4] * Y[k-4];
                    c += arr[i][k-3] * Y[k-3];
                    c += arr[i][k-2] * Y[k-2];
                    c += arr[i][k-1] * Y[k-1];
                    c += arr[i][k] * Y[k];
                }
                if ( col < 8 ) {
                    for(int k = 0; k < col; ++k) {
                        c += arr[i][k] * Y[k];
                    }
                }
                result.arr[i][j] = c;
            }
        }
        //result.print();
        return true;
        
    }
}
