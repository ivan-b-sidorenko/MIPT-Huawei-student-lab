#include "tensor.hpp"



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

    bool Matrix::add_matrix(const linear::Matrix& A) {
        if ( ( row != A.get_row() ) & ( col != A.get_col() ) ) {
            return false;
        }
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] += A.arr[i][j];
            }
        }
        return true;
    }

    /*               Здесь небольшие ускорения, связанные с выносом переменной за тело цикла(ускорение для наивной около 4%)
    float opt_count(linear::Matrix& A, linear::Matrix& Ker, int i, int j) {
        float sum = 0;
        for(int k = i; k < i + 4; ++k) {
            for(int p = j; p < j + 4; ++p) {
                sum += A.arr[k][p] * Ker.arr[k - i][p - j];
            }
        }   
        return sum;
    }
    */    

    float count(linear::Matrix& A, linear::Matrix& Ker, int i, int j) {
        float sum = 0;
        for(int k = i; k < i + 4; ++k) {
            for(int p = j; p < j + 4; ++p) {
                sum += A.arr[k][p] * Ker.arr[k - i][p - j];
            }
        }
        return sum; 
    }


    linear::Matrix& Matrix::matrix_conv(std::vector<linear::Matrix>& A, std::vector<linear::Matrix>& Kernel, linear::Matrix& result) {
        linear::Matrix result1(result.get_row(), result.get_col(), 0.0);
        for(int k = 0; k < Kernel.size(); ++k) {
            for(int i = 0; i < result.get_row(); ++i) {
                for(int j = 0; j < result.get_col(); ++j) {
                    result1.arr[i][j] = count(A[k], Kernel[k], i, j);
                }
            }
            result.add_matrix(result1);
        }
        return result;
    }
    
    /*          Добавлю позже 
    linear::Matrix& Matrix::Matrix::matrix_opt_conv(std::vector<linear::Matrix>& A, std::vector<linear::Matrix>& Kernel, linear::Matrix& result) {
        int R = result.get_row();
        int C = result.get_col();
        for(int i = 0; i < R; ++i) {
            for(int j = 0; j < C; ++j) {
                //result.arr[i][j] = opt_count(A, Kernel, i, j);
            }
        }
        return result;
    }
    */
    bool Matrix::compare(linear::Matrix& M) const {
        for(int i = 0; i < M.get_row(); ++i) {
            for(int j = 0; j < M.get_col(); ++j) {
                if ( ( arr[i][j] - M.arr[i][j] ) > 0.00001 ) {
                    return false;
                }
            }
        }
        return true;
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

    bool Batch::compare(ML::Batch& bat) const {
        for(int i = 0; i < Matrixs.size(); ++i) {
            if ( Matrixs[i].compare(bat.Matrixs[i]) == false ) {
                return false;
            }
        }
        return true;
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
    
    bool Tensor::compare(ML::Tensor& tensor) const {
        if ( width == tensor.get_width() & ( height == tensor.get_height() ) & ( channel == tensor.get_channel() ) & ( num_batch == tensor.get_num_batch() ) ) {
            for(int i = 0; i < batches.size(); ++i) {
                if ( batches[i].compare(tensor.batches[i]) == false ) {
                    return false;
                }
            }
            return true;
        }
        return false;
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
        for(int j = 0; j < result.get_num_batch(); ++j) {
            for(int i = 0; i < Kernel.get_num_batch(); ++i) {
                result.batches[j].Matrixs[i].matrix_conv(batches[j].Matrixs, Kernel.batches[i].Matrixs, result.batches[j].Matrixs[i]);
            }
        }
        return result;
    }
    
    void mini_opt_conv(int begin, int end, std::vector<linear::Matrix> Matrixs, ML::Tensor Kernel, std::vector<linear::Matrix>& M) {
        int row = Matrixs[0].get_row();
        int col = Matrixs[0].get_col();
        linear::Matrix matr(row - 3, col - 3, 0.0);
        for(int i = begin; i < end; ++i) {
            matr = matr.matrix_conv(Matrixs, Kernel.batches[i].Matrixs, matr);
            M.push_back(matr);      
        }
    }
    
    void Batch::union_matrix(std::vector<linear::Matrix> M1, std::vector<linear::Matrix> M2, std::vector<linear::Matrix> M3, std::vector<linear::Matrix> M4) {
        int M1_size = M1.size();
        int M2_size = M2.size();
        int M3_size = M3.size();
        int M4_size = M4.size();
        for(int i = 0; i < M1_size; ++i) {
            Matrixs[i].add_matrix(M1[i]);       //изначально матрицы проиницилизированы нулем, поэтому складываем их с результатами
        }
        for(int i = 0; i < M2_size; ++i) {
            Matrixs[i + M1_size].add_matrix(M2[i]);
        }
        for(int i = 0; i < M3_size; ++i) {
            Matrixs[i + M1_size + M2_size].add_matrix(M3[i]);
        }
        for(int i = 0; i < M4_size; ++i) {
            Matrixs[M1_size + M2_size + M3_size + i].add_matrix(M4[i]);
        }
    }

    ML::Tensor& Tensor::opt_conv(ML::Tensor& Kernel, ML::Tensor& result) {
        int res_num = result.get_num_batch();
        int ker_num = Kernel.get_num_batch();
        if ( ker_num < 4 ) {
            result = this->conv(Kernel, result);
            return result;
        }
        std::vector<linear::Matrix> M = batches[0].Matrixs;         //num batch = 1

        ML::Tensor ker(Kernel);
        
        std::vector<linear::Matrix> M1;
        std::vector<linear::Matrix> M2;
        std::vector<linear::Matrix> M3;
        std::vector<linear::Matrix> M4;
        
        std::vector<std::thread> threads;
        threads.push_back(std::thread(mini_opt_conv, 0, ker_num / 4, M, std::ref(ker), std::ref(M1)));
        threads.push_back(std::thread(mini_opt_conv, ker_num / 4, 2 * (ker_num / 4), M, std::ref(ker), std::ref(M2)));
        threads.push_back(std::thread(mini_opt_conv, 2 * (ker_num / 4), 3 * (ker_num / 4), M, std::ref(ker), std::ref(M3)));
        threads.push_back(std::thread(mini_opt_conv, 3 * (ker_num / 4), ker_num, M, std::ref(ker), std::ref(M4)));
        for(std::thread& t : threads) {
            t.join();
        }
        result.batches[0].union_matrix(M1, M2, M3, M4);
        return result;
    }
}
        
