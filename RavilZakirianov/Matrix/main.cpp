#include "matrix.hpp"

int** init(int** arr, int m, int n) {
    arr = new int*[m];
    for(int i = 0; i < m; ++i) {
        arr[i] = new int[n];
    }
    return arr;
}


void mul(linear::Matrix& A, linear::Matrix& B, linear::Matrix& result) {
    bool p = A.mul_impr(B, result);
    if ( p == 0 ) {
        exit(1);
    }
}

void TEST1(linear::Matrix& A, linear::Matrix& B) {
    std::vector<int> Time1;
    linear::Matrix result(A.get_row(), B.get_col());
    for(int i = 0; i < 1; ++i) {
        auto begin = std::chrono::steady_clock::now();
        bool p = A.mul(B, result);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        Time1.push_back(elapsed_ms.count());

    }
    int time = 0;
    for(int i = 0; i < Time1.size(); ++i) {
        time += Time1[i];
    }
    //result.print();
    result.record("OUT_TEST1");
    std::cout << "The first time: " << time / Time1.size() << " ms\n";
}

void TEST2(linear::Matrix& A, linear::Matrix& B) {
    std::vector<int> Time1;
    linear::Matrix result(A.get_row(), B.get_col());
    for(int i = 0; i < 1; ++i) {
        auto begin = std::chrono::steady_clock::now();
        bool p = A.mul_impr(B, result);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        Time1.push_back(elapsed_ms.count());
    }
    int time = 0;
    for(int i = 0; i < Time1.size(); ++i) {
        time += Time1[i];
    }
    //result.print();
    result.record("OUT_TEST2");
    std::cout << "The optimization time: " << time / Time1.size() << " ms\n";
}

void TEST3(linear::Matrix& A1, linear::Matrix& B1) {
    std::vector<std::thread> threads;
    std::vector<int> Time;
    int my_row = A1.get_row();
    int my_col = B1.get_col();
    linear::Matrix A2(A1);                              //Создаем 8 матриц, чтобы запустить их умножение параллельно
    linear::Matrix A3(A1);
    linear::Matrix A4(A1);
    linear::Matrix B2(B1);
    linear::Matrix B3(B1);
    linear::Matrix B4(B1);
    linear::Matrix result1(my_row, my_col);
    linear::Matrix result2(my_row, my_col);
    linear::Matrix result3(my_row, my_col);
    linear::Matrix result4(my_row, my_col);
    auto begin = std::chrono::steady_clock::now();
    threads.push_back(std::thread(mul, std::ref(A1), std::ref(B1), std::ref(result1)));
    threads.push_back(std::thread(mul, std::ref(A2), std::ref(B2), std::ref(result2)));
    threads.push_back(std::thread(mul, std::ref(A3), std::ref(B3), std::ref(result3)));
    threads.push_back(std::thread(mul, std::ref(A4), std::ref(B4), std::ref(result4)));
    for(std::thread& t : threads) {
        t.join();
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        Time.push_back(elapsed_ms.count());
    }

    int max_time = 0;
    for(int i = 0; i < Time.size(); ++i) {
        if ( Time[i] > max_time ) {
            max_time = Time[i];
        }
    }
    result1.record("OUT_TEST3");
    //result1.print();
    //result2.print();
    //result3.print();
    //result4.print();
    std::cout << "The optimization + middle threads time(linear equation): " << max_time / Time.size() << " ms\n";
    //std::cout << std::thread::hardware_concurrency() << std::endl;
}

void TEST4(linear::Matrix& A, linear::Matrix& B) {
    std::vector<int> Time;
    linear::Matrix result(A.get_row(), B.get_col());
    for(int i = 0; i < 4; ++i) {
        auto begin = std::chrono::steady_clock::now();
        bool p = A.mul_thread(B, result);
        auto end = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        Time.push_back(elapsed_ms.count());
    }
    int time = 0;
    for(int i = 0; i < Time.size(); ++i) {
        time += Time[i];
    }
    result.record("OUT_TEST4");
    std::cout << "The optimization + mul_thread time: " << time / Time.size() << " ms\n";
}



int main() {
    int col, col2, row, row2;
    std::cin >> row >> col;
    std::cin >> row2 >> col2;
    if ( ( col < 1 ) | ( col2 < 1 ) | ( row < 1 ) | ( row2 < 1 ) ) {
        std::cout << "Incorrect size\n";
        return 1;
    }
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
    TEST4(A, B);
    for(int i = 0; i < row; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
    for(int i = 0; i < row2; ++i) {
        delete[] arr2[i];
    }
    delete[] arr2;
}

