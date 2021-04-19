#pragma once

#include <iostream>
#include <vector>
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()
#include <cstdlib> // для функций rand() и srand()
#include <chrono>
#include <thread>
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
            Matrix(int row, int col, linear::Matrix& A);        //im2col
            Matrix(int row, int col, std::vector<linear::Matrix>& Matrixs);
            bool add_matrix(const linear::Matrix& A);
            void transpone(const linear::Matrix& A);
            int get_row() const;
            int get_col() const;
            bool compare(linear::Matrix& M) const;
            void print() const;
            Matrix& matrix_intr_mul(const linear::Matrix& B, linear::Matrix& result);
            void matrix_mul(const linear::Matrix& B, linear::Matrix& result);
            void matrix_mul_thread(const linear::Matrix& right, linear::Matrix& result);
            Matrix& matrix_conv(std::vector<linear::Matrix>& input, std::vector<linear::Matrix>& Ker, Matrix& result);
            Matrix& matrix_opt_conv(std::vector<linear::Matrix>& input, std::vector<linear::Matrix>& Ker, Matrix& result);
            Matrix& win_matrix_conv(std::vector<linear::Matrix>& input, std::vector<linear::Matrix>& Ker, Matrix& result);
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
            bool compare(ML::Batch& batch) const;
            void union_matrix(std::vector<linear::Matrix> M1, std::vector<linear::Matrix> M2, std::vector<linear::Matrix> M3, std::vector<linear::Matrix> M4);
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
            ML::Tensor& opt_conv(Tensor& Kernel, Tensor& result);
            ML::Tensor& winograd_conv(Tensor& Kernel, Tensor& result);
            void print() const;
            bool compare(ML::Tensor& tensor) const;
            ML::Tensor& gemm(ML::Tensor& Kernel, ML::Tensor& result, linear::Matrix& M2);
            int get_width() const;
            int get_height() const;
            int get_channel() const;
            int get_num_batch() const;
    };
}
