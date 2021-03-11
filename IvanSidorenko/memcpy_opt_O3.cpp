// Compilation: clang++ memcpy_opt_O3.cpp -c -std=c++14 -O3 -mavx -o memcpy_opt_O3.o
#include <cstdint>

// Load and Store by 1 byte
void memcpy_opt_O3(void * dst, void * src, std::size_t n)
{
    uint8_t * d = static_cast<uint8_t*>(dst);
    uint8_t * s = static_cast<uint8_t*>(src);

    for (std::size_t i = 0; i < n; i++) {
        *d++ = *s++;
    }
}
