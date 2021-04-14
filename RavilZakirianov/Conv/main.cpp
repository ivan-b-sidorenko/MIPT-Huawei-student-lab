#include "tensor.hpp"

float** init(float** arr, int m, int n) {
    arr = new float*[m];
    for(int i = 0; i < m; ++i) {
        arr[i] = new float[n];
    }
    return arr;
}

void TEST1(int K, int C, int H, int W, float** arr, linear::Matrix& M, ML::Tensor& result) {
    linear::Matrix Ker(4, 4, arr);
    ML::Batch inp(M, C);
    ML::Batch ker(Ker, C);
    ML::Tensor input(inp, 1);
    ML::Tensor kernel(ker, K);
    auto begin = std::chrono::steady_clock::now();
    result = input.conv(kernel, result);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The first time: " << elapsed_ms.count() << " ms\n";
}

void TEST2(int K, int C, int H, int W, float** arr, linear::Matrix& M, ML::Tensor& result) {
    linear::Matrix Ker(4, 4, arr);
    ML::Batch inp(M, C);
    ML::Batch ker(Ker, C);
    ML::Tensor input(inp, 1);
    ML::Tensor kernel(ker, K);
    auto begin = std::chrono::steady_clock::now();
    result = input.opt_conv(kernel, result);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The optimization thread  time: " << elapsed_ms.count() << " ms\n";
}

void TEST3(int K, int C, int H, int W, float** arr, linear::Matrix& M, ML::Tensor& result) {
    linear::Matrix Ker(4, 4, arr);
    ML::Batch inp(M, C); 
    ML::Batch ker(Ker, C); 
    ML::Tensor input(inp, 1); 
    ML::Tensor kernel(ker, K); 
    auto begin = std::chrono::steady_clock::now();
    result = input.gemm(kernel, result);
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The optimization Gemm time: " << elapsed_ms.count() << " ms\n";
}


void compare(ML::Tensor& result1, ML::Tensor& result2, ML::Tensor& result3) {
    if ( result1.compare(result2) == true ) {
        std::cout << "TEST 1 OK" << std::endl;
    }
    else {
        std::cout << "TEST 1 NOT OK" << std::endl;
    }
    if ( result1.compare(result3) == true ) {
        std::cout << "TEST 2 OK" << std::endl;
    }
    else {
        std::cout << "TEST 2 NOT OK" << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int N, K, C, H, W;
    N = 1;
    std::cin >> K >> C >> H >> W;
    linear::Matrix M(W, H);
    if ( ( M.get_row() < 5 ) & ( M.get_col() < 5 ) ) {
        std::cout << "Small Size\n";
        return 1;
    }
    linear::Matrix M1(W - 3, H - 3, 0.0);
    ML::Batch out(M1, K);
    ML::Tensor result1(out, 1);
    ML::Tensor result2(out, 1);
    ML::Tensor result3(out, 1);
    float** arr = nullptr;
    arr = init(arr, 4, 4);
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            arr[i][j] = 0.5;
        }
    }
    TEST1(K, C, H, W, arr, M, result1);
    TEST2(K, C, H, W, arr, M, result2);
    TEST3(K, C, H, W, arr, M, result3);
    compare(result1, result2, result3);
    for(int i = 0; i < 4; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
    return 0;
}
