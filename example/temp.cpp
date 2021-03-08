#include <iostream>

template <typename T>
void foo(T size) {
    for(T i = 0; i < size; ++i) {
        T data = i;
        T data_j = 0;
        T data_p = 1;
    }
}


int main() {
    foo(100000000);
}
