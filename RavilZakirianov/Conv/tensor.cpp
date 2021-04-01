#include "tensor.hpp"
#include <iostream>
#include <vector>

namespace linear {
    Matrix::Matrix(int _row, int _col) {
        row = _row;
        col = _col;
        arr = new float*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new float[col];
        }
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] = rand() % 10;
            }
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
    
    Matrix::Matrix(int _row, int _col, float flag) {
        row = _row;
        col = _col;
        arr = new float*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new float[col];
        }
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] = flag;
            }   
        }   
    }

    Matrix::Matrix(int _row, int _col, float** _arr) {
        row = _row;
        col = _col;
        arr = new float*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new float[col];
        }
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] = _arr[i][j];
            }
        }
    }

    Matrix::Matrix(const linear::Matrix& A) {
        row = A.get_row();
        col = A.get_col();
        arr = new float*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new float[col];
        }
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] = A.arr[i][j];
            }
        }
    }
    
    float count(linear::Matrix& A, linear::Matrix& Ker, int i, int j) {
        float sum = 0;
        for(int k = i; k < i + 4; ++k) {
            for(int p = j; p < j + 4; ++p) {
                sum += A.arr[k][p] * Ker.arr[k - i][p - j];
            }
        }   
        return sum;
    }


    linear::Matrix& Matrix::matrix_conv(linear::Matrix& A, linear::Matrix& Kernel, linear::Matrix& result) {
        for(int i = 0; i < result.get_row(); ++i) {
            for(int j = 0; j < result.get_col(); ++j) {
                result.arr[i][j] = count(A, Kernel, i, j);
            }
        }   
        return result;
    }


    int Matrix::get_row() const {
        return row;
    }

    int Matrix::get_col() const {
        return col;
    }

    Matrix::~Matrix() {
        for(int i = 0; i < row; ++i) {
            delete[] arr[i];
        }
        delete[] arr;
    }
}

namespace ML {
    Batch::Batch(int w, int h, int c) {
        width = w;
        height = h;
        channel = c;
        Matrixs.push_back(linear::Matrix(w, h));
    }

    Batch::Batch(linear::Matrix& A) {
        width = A.get_row();
        height = A.get_col();
        channel = 1;
        Matrixs.push_back(A);
    }
    
    Batch::Batch(linear::Matrix& A, int c) {
        for(int i = 0; i < c; ++i) {
            Matrixs.push_back(A);
        }
        channel = c;
        width = A.get_row();
        height = A.get_col();
    }

    Batch::Batch(std::vector<linear::Matrix> matrixs, int w, int c, int h) {
       Matrixs = matrixs;
       width = w;
       height = h;
       channel = c;
    }

    int Batch::get_width() const {
        return width;
    }

    int Batch::get_height() const {
        return height;
    }

    int Batch::get_channel() const {
        return channel;
    }

    void Batch::print() const {
        for(auto M : Matrixs) {
            M.print();
            std::cout << "NEXT_CHANNEL\n";
        }
    }

    Tensor::Tensor(int w, int h, int c, int num_b) {
        width = w;
        height = h;
        channel = c;
        num_batch = num_b;
        for(int i = 0; i < num_b; ++i) {
            for(int j = 0; j < channel; ++j) {
            }
        }

            
    }

    Tensor::Tensor(const ML::Batch& B, int num_b) {
        num_batch = num_b;
        for(int i = 0; i < num_b; ++i) {
            batches.push_back(B);
        }
        width = B.get_width();
        height = B.get_height();
        channel = B.get_channel();
    }
    
    Tensor::Tensor(int w, int h, int c, int num_b, std::vector<ML::Batch> B) {
        num_batch = num_b;
        width = w;
        height = h;
        channel = c;
        batches = B;
    }

    int Tensor::get_width() const {
        return width;
    }

    int Tensor::get_height() const {
        return height;
    }

    int Tensor::get_channel() const {
        return channel;
    }

    int Tensor::get_num_batch() const {
        return num_batch;
    }

    void Tensor::print() const {   
        for(auto t : batches) {
            t.print();
            std::cout << "NEXT_BATCH\n";
        }
    }

    ML::Tensor& Tensor::conv(ML::Tensor& Kernel, ML::Tensor& result) {
        for(int i = 0; i < num_batch; ++i) {
            for(int j = 0; j < channel; ++j) {
                result.batches[i].Matrixs[j].matrix_conv(batches[i].Matrixs[j], Kernel.batches[i].Matrixs[j], result.batches[i].Matrixs[j]);
            }
        }
        return result;
    }
}
