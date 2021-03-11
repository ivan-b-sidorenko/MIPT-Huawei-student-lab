#include "matrix.hpp"

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

void TEST1(linear::Matrix& A, linear::Matrix& B) {
    std::vector<int> Time1;
    for(int i = 0; i < 1; ++i) {
        auto begin = std::chrono::steady_clock::now();
        bool p = A + B;
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        Time1.push_back(elapsed_ms.count());

    }
    int time = 0;
    for(int i = 0; i < Time1.size(); ++i) {
        time += Time1[i];
    }
    std::cout << "The first time: " << time / Time1.size() << " ms\n";
}

void TEST2(linear::Matrix& A, linear::Matrix& B) {
    std::vector<int> Time1;
    for(int i = 0; i < 1; ++i) {
        auto begin = std::chrono::steady_clock::now();
        bool p = A * B;
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        Time1.push_back(elapsed_ms.count());
    }
    int time = 0;
    for(int i = 0; i < Time1.size(); ++i) {
        time += Time1[i];
    }
    std::cout << "The optimization time: " << time / Time1.size() << " ms\n";
}

void TEST3(linear::Matrix& A, linear::Matrix& B) {
    std::vector<std::thread> threads;
    
    for(int i = 0; i < 4; ++i) {
        threads.push_back(std::thread(mul, std::ref(A), std::ref(B), i));
    }
    for(std::thread& t : threads) {
        t.join();
    }
    int max_time = 0;
    for(int i = 0; i < Time.size(); ++i) {
        if ( Time[i] > max_time ) {
            max_time = Time[i];
        }
    }
    std::cout << "The optimization + threads time: " << max_time / Time.size() << " ms\n";
    //std::cout << std::thread::hardware_concurrency() << std::endl;
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
    TEST1(A, B);
    TEST2(A, B);
    TEST3(A, B);
    for(int i = 0; i < row; ++i) {
        delete[] arr[i];
    }
    for(int i = 0; i < row2; ++i) {
        delete[] arr2[i];
    }
}

