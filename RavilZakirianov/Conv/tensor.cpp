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
    

    Matrix::Matrix(int _row, int _col, std::vector<linear::Matrix>& A) {
        row = _row;
        col = _col;
        int chan = A.size();
        arr = new float*[row];
        for(int i = 0; i < row; ++i) {
            arr[i] = new float[col];
        }
        int A_ROW = A[0].get_row() - 3;
        int A_COL = A[0].get_col() - 3;
        for(int k = 0; k < chan; ++k) {
            int num_i = 0;
            int num_j = 0;
            for(int i = 0; i < A_ROW; ++i) {
                for(int j = 0; j < ( A_COL ); ++j) {
                    for(int start_num_i = i; start_num_i < i + 4; ++start_num_i) {
                        for(int start_num_j = j; start_num_j < j + 4; ++start_num_j) {
                            arr[num_i][num_j + (k * col / chan)] = A[k].arr[start_num_i][start_num_j];
                            num_i++;
                        }
                    }
                    num_i = 0;
                    num_j++;
                }
            }
        }
    }
    
    void Matrix::transpone(const linear::Matrix& A) {
        for(int i = 0; i < row; ++i) {
            for(int j = 0; j < col; ++j) {
                arr[i][j] = A.arr[j][i];
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
        
    Matrix& Matrix::matrix_intr_mul(const linear::Matrix& B, linear::Matrix& result) {
        //add
        return result;

    }

     void mini_mul(const linear::Matrix& A, const linear::Matrix& B, int m1, int m2, int k1, int k2, int len, linear::Matrix& result) {
        int M = result.get_row();
        int N = result.get_col();
        int num = k1;
        float c = 0;
        for(int j = 0; j < N; ++j) {
            float Y[len];           //столбец
            for(int q = 0; q < len; ++q) {
                Y[q] = B.arr[q][num];
            }
            num++;
            for(int i = 0; i < M; ++i) {            //ускорение перестановкой циклов, вынос переменных
                c = 0;
                int k = 7;
                while ( k < len ) {                 //loop unrolling
                    c += A.arr[m1 + i][k-7] * Y[k-7];
                    c += A.arr[m1 + i][k-6] * Y[k-6];
                    c += A.arr[m1 + i][k-5] * Y[k-5];
                    c += A.arr[m1 + i][k-4] * Y[k-4];
                    c += A.arr[m1 + i][k-3] * Y[k-3];
                    c += A.arr[m1 + i][k-2] * Y[k-2];
                    c += A.arr[m1 + i][k-1] * Y[k-1];
                    c += A.arr[m1 + i][k] * Y[k];
                    k += 8;
                }
                k -= 8;
                if ( len < 8 ) {                                              //ch
                    for(int ik = 0; ik < len; ++ik) {
                        c += A.arr[m1 + i][ik] * Y[ik];
                    }
                }
                else if ( len % 8 != 0 ) {
                    for(int ik = k + 1; ik < len; ++ik) {
                        c += A.arr[m1 + i][ik] * Y[ik];
                    }
                }
                result.arr[i][j] = c;
            }
        }
    }

    void union_matrix(linear::Matrix& result, linear::Matrix& res1, linear::Matrix& res2, linear::Matrix& res3, linear::Matrix& res4) {
        int m1 = res1.get_row();                                                                    //WORK
        int k1 = res1.get_col();
        int m = result.get_row();
        int k = result.get_col();
        int k2 = res2.get_col();
        for(int i = 0; i < m1; ++i) {
            for(int j = 0; j < k1; ++j) {
                result.arr[i][j] = res1.arr[i][j];
            }
        }
        for(int i = 0; i < m1; ++i) {
            for(int j = k1; j < k; ++j) {
                result.arr[i][j] = res2.arr[i][j - k1];
            }
        }
        for(int i = m1; i < m; ++i) {
            for(int j = 0; j < k1; ++j) {
                result.arr[i][j] = res3.arr[i - m1][j];
            }
        }
        for(int i = m1; i < m; ++i) {
            for(int j = k1; j < k; ++j) {
                result.arr[i][j] = res4.arr[i - m1][j - k1];
            }
        }
    }

    void Matrix::matrix_mul_thread(const linear::Matrix& right, linear::Matrix& result) {
        std::vector<std::thread> threads;
        int m1 = row / 2;
        int m2 = row - m1;
        int k1 = right.get_col() / 2;
        int k2 = right.get_col() - k1;
        int len = right.row;
        linear::Matrix left(row, col, arr);
        linear::Matrix result1(m1, k1);
        linear::Matrix result2(m1, k2);
        linear::Matrix result3(m2, k1);
        linear::Matrix result4(m2, k2);
        threads.push_back(std::thread(mini_mul, std::ref(left), std::ref(right), 0, m1, 0, k1, len, std::ref(result1)));
        threads.push_back(std::thread(mini_mul, std::ref(left), std::ref(right), 0, m1, k1, k1 + k2, len, std::ref(result2)));
        threads.push_back(std::thread(mini_mul, std::ref(left), std::ref(right), m1, 0, 0, k1, len, std::ref(result3)));
        threads.push_back(std::thread(mini_mul, std::ref(left), std::ref(right), m1, m1 + m2, k1, k1 + k2, len, std::ref(result4)));
        for(std::thread& t : threads) {
            t.join();
        }
        union_matrix(result, result1, result2, result3, result4);
    }

    void Matrix::matrix_mul(const linear::Matrix& right, linear::Matrix& result) {
        int M = result.get_row();
        int N = result.get_col();
        int num = 0;
        float c = 0;
        for(int j = 0; j < N; ++j) {
            float Y[col];           //столбец
            for(int q = 0; q < col; ++q) {
                Y[q] = right.arr[q][num];
            }
            num++;
            for(int i = 0; i < M; ++i) {            //ускорение перестановкой циклов, вынос переменных
                c = 0;
                int k = 7;
                while ( k < col ) {                 //loop unrolling
                    c += arr[i][k-7] * Y[k-7];
                    c += arr[i][k-6] * Y[k-6];
                    c += arr[i][k-5] * Y[k-5];
                    c += arr[i][k-4] * Y[k-4];
                    c += arr[i][k-3] * Y[k-3];
                    c += arr[i][k-2] * Y[k-2];
                    c += arr[i][k-1] * Y[k-1];
                    c += arr[i][k] * Y[k];
                    k += 8;
                }
                k -= 8;
                if ( col < 8 ) {                                              //ch
                    for(int ik = 0; ik < col; ++ik) {
                        c += arr[i][ik] * Y[ik];
                    }
                }
                else if ( col % 8 != 0 ) { 
                    for(int ik = k + 1; ik < col; ++ik) {
                        c += arr[i][ik] * Y[ik];
                    }
                }
                result.arr[i][j] = c;
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
    
    float sum_filter(linear::Matrix& A, int num, int num_ker, int step) {
        float reply = 0.0;
        int A_COL = A.get_col();
        for(int i = num; i < A_COL; i += step) {
            reply += A.arr[0][i];
        }
        return reply;
    }

    ML::Tensor& col2im(linear::Matrix& A, ML::Tensor& result) {
        int num = 0;
        int W = result.get_width();
        int H = result.get_height();
        int C = result.get_channel();
        for(int k = 0; k < C; ++k) {
            for(int i = 0; i < W; ++i) {
                for(int j = 0; j < H; ++j) {
                    result.batches[0].Matrixs[k].arr[i][j] = sum_filter(A, num, k, W * H);
                    num++;
                }
            }
            num = 0;
        }
        return result;

    }
    
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
    

    ML::Tensor& Tensor::gemm(ML::Tensor& Kernel, ML::Tensor& result, linear::Matrix& M2) {
        int col2 = ( height - Kernel.get_height() + 1 ) * ( width - Kernel.get_width() + 1 ) * channel;
        int row1 = Kernel.get_num_batch();
        linear::Matrix M1(Kernel.get_width() * Kernel.get_height(), col2, batches[0].Matrixs);
        linear::Matrix result1(M2.get_row(), M1.get_col(), 0.0);
        M2.matrix_mul_thread(M1, result1);
        result = col2im(result1, result);
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
        
