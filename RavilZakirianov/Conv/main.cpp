#include "tensor.hpp"

float** init(float** arr, int m, int n) {
    arr = new float*[m];
    for(int i = 0; i < m; ++i) {
        arr[i] = new float[n];
    }
    return arr;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int N, C, H, W;
    std::cin >> N >> C >> H >> W;
    linear::Matrix M(W, H);
    if ( ( M.get_row() < 5 ) & ( M.get_col() < 5 ) ) {
        std::cout << "Small Size\n";
        return 1;
    }
    float** arr = nullptr;
    arr = init(arr, 4, 4);
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            arr[i][j] = 0.5;
        }
    }
    linear::Matrix Ker(4, 4, arr);
    linear::Matrix outp(M.get_row() - 3, M.get_col() - 3, 0.99);
    ML::Batch inp(M, C);
    ML::Batch ker(Ker, C);
    ML::Batch out(outp, C);                         
    ML::Tensor input(inp, N);
    ML::Tensor kernel(ker, N);
    ML::Tensor result(out, N);
    result = input.conv(kernel, result);
    result.print();
    for(int i = 0; i < 4; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
    return 0;
}
