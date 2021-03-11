#include "matrix_2.hpp"

int** init(int** arr, int m, int n) {
    arr = new int*[m];
    for(int i = 0; i < m; ++i) {
        arr[i] = new int[n];
    }
    return arr;
}

std::vector<int> Time;

void mul(linear::Matrix A, linear::Matrix B, int i) {
    auto begin = std::chrono::steady_clock::now();
    bool p = A * B;
    if ( p == 0 ) {
        exit(1);
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    Time.push_back(elapsed_ms.count());
}


int main() {
    int col, col2, row, row2;
    std::cin >> row >> col;
    std::cin >> row2 >> col2;
    srand(static_cast<unsigned int>(time(0)));
    int** arr = nullptr;
    arr = init(arr, row, col);
    int** arr2 = nullptr;
    arr2 = init(arr2, row2, col2);
    
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < col; j++) {
            arr[i][j] = rand() % 10;
        }
    }
    
    for(int i = 0; i < row2; ++i) {
        for(int j = 0; j < col2; j++) {
            arr2[i][j] = rand() % 10;
        }   
    } 
    linear::Matrix A(row, col, arr);
    linear::Matrix B(row2, col2, arr2);
    //A.print();
    //std::cout << std::endl;
    //B.print();
    //std::cout << std::endl;
    /*std::cout << "A" << ": \n";               //удобно использовать для маленьких тестов, чтобы проверить правильность алгоритма
    A.print();
    std::cout << "\n B : \n";
    B.print();
    std::cout << std::endl << "MUL\n";
    */
    //std::vector<int> Time;
    /*for(int i = 0; i < 10; ++i) {
        auto begin = std::chrono::steady_clock::now();
        bool p = A * B;
        if ( p == 0 ) {
            return -1;
        }
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        Time.push_back(elapsed_ms.count());
    }
    */
    for(int i = 0; i < 4; ++i) {
        std::thread t(mul, std::ref(A), std::ref(B), i);
        t.join();
    }
    int time = 0;
    for(int i = 0; i < Time.size(); ++i) {
        time += Time[i];
    }
    std::cout << "The time: " << time / Time.size() << " ms\n";
    for(int i = 0; i < row; ++i) {
        delete[] arr[i];
    }
    for(int i = 0; i < row2; ++i) {
        delete[] arr2[i];
    }
}
