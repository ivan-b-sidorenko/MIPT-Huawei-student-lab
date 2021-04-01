#pragma once

#include <iostream>
#include <vector>
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()

#define p(s) std::cout << s << std::endl;

namespace linear {
    
    class Matrix {
        private:
            int row;
            int col;
        public:
            float** arr;
            Matrix(int row, int col);
            Matrix(int row, int col, float flag);
            Matrix(int row, int col, float** arr);
            Matrix(const linear::Matrix& A);
            int get_row() const;
            int get_col() const;
            void print() const;
            Matrix& matrix_conv(Matrix& input, Matrix& Ker, Matrix& result);
            ~Matrix();
    };
}

namespace ML {
    
    class Batch {
        private:
            int width;
            int height;
            int channel;
        public:
            std::vector<linear::Matrix> Matrixs;
            Batch(int w, int h, int c);
            Batch(linear::Matrix& matrix);
            Batch(linear::Matrix& matrix, int c);
            Batch(std::vector<linear::Matrix> matrix, int w, int h, int c);
            void print() const;
            int get_width() const;
            int get_height() const;
            int get_channel() const;
    };

    class Tensor {
        private:
            int width;
            int height;
            int channel;
            int num_batch;
        public:
            std::vector<ML::Batch> batches;
            Tensor(int w, int h, int c, int num_b);
            Tensor(int w, int h, int c, int num_b, std::vector<ML::Batch> B);
            Tensor(const ML::Batch& b, int num_b);
            ML::Tensor& conv(Tensor& Kernel, Tensor& result);
            void print() const;
            int get_width() const;
            int get_height() const;
            int get_channel() const;
            int get_num_batch() const;
    };
}
