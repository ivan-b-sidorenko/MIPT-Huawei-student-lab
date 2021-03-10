// Compilation: clang++ memcpy.cpp -c -o memcpy.o -std=c++14 -O0 -mavx
// Linking: clang++ memcpy.o memcpy_opt_O3.o -o memcpy.exe

#include <iostream>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>
#include <thread>

#define NUM 200000000
#define EXPERIMENTS_NUM 10
#define THREADS_NUM 8

typedef void (*FP) (void * dst, void * src, size_t n);

void memcpy_opt_O3(void * dst, void * src, size_t n);

// Load and Store by 1 byte
void memcpy_algo1(void * dst, void * src, size_t n)
{
    uint8_t * d = static_cast<uint8_t*>(dst);
    uint8_t * s = static_cast<uint8_t*>(src);

    for (size_t i = 0; i < n; i++) {
        *d++ = *s++;
    }
}

// Load and Store by 8 bytes
void memcpy_algo2(void * dst, void * src, size_t n)
{
    uint64_t * d = static_cast<uint64_t*>(dst);
    uint64_t * s = static_cast<uint64_t*>(src);
    size_t nn = n / sizeof(uint64_t);

    for (size_t i = 0; i < nn; i++) {
        *d++ = *s++;
    }
}

// Load and Store by 8 bytes + unroll 2x
void memcpy_algo3(void * dst, void * src, size_t n)
{
    uint64_t * d = static_cast<uint64_t*>(dst);
    uint64_t * s = static_cast<uint64_t*>(src);
    size_t nn = n / sizeof(uint64_t);

    for (size_t i = 0; i < nn/2; i++) {
        *d = *s;
        *(d+1) = *(s+1);
        d += 2;
        s += 2;
    }
}

// Load and Store by 8 bytes + unroll 4x
void memcpy_algo4(void * dst, void * src, size_t n)
{
    uint64_t * d = static_cast<uint64_t*>(dst);
    uint64_t * s = static_cast<uint64_t*>(src);
    size_t nn = n / sizeof(uint64_t);

    for (size_t i = 0; i < nn/4; i++) {
        *d = *s;
        *(d+1) = *(s+1);
        *(d+2) = *(s+2);
        *(d+3) = *(s+3);
        d += 4;
        s += 4;
    }
}

#include <immintrin.h>

// Load and Store by 32 bytes
void memcpy_algo5(void * dst, void * src, size_t n)
{
    __m256i * s = static_cast<__m256i*>(src);
    __m256i * d = static_cast<__m256i*>(dst);
    size_t nn = n / sizeof(__m256i);

    for (size_t i = 0; i < nn; i++) {
        __m256i integer_vector = _mm256_load_si256(s++);
        _mm256_store_si256(d++, integer_vector);
    }
}

// Load and Store by 32 bytes + unroll 2x
void memcpy_algo6(void * dst, void * src, size_t n)
{
    __m256i * s = static_cast<__m256i*>(src);
    __m256i * d = static_cast<__m256i*>(dst);
    size_t nn = n / sizeof(__m256i);

    for (size_t i = 0; i < nn/2; i++) {
        __m256i integer_vector0 = _mm256_load_si256(s);
        _mm256_store_si256(d, integer_vector0);
        __m256i integer_vector1 = _mm256_load_si256(s+1);
        _mm256_store_si256(d+1, integer_vector1);
        s += 2;
        d += 2;
    }
}

// Load and Store by 1 byte run in parallel 8 threads
void memcpy_algo7(void * dst, void * src, size_t n)
{
    std::vector<std::thread> threads;
    uint8_t * d = static_cast<uint8_t*>(dst);
    uint8_t * s = static_cast<uint8_t*>(src);
    size_t NN = n / THREADS_NUM;

    for (size_t i = 0; i < THREADS_NUM; i++) {
        threads.push_back(std::thread(memcpy_algo1, d + NN*i, s + NN*i, NN));
    }

    for (std::thread& t : threads) {
        t.join();
    }
}

uint64_t time(void * dst, void * src, size_t n, FP algo) {
    std::vector<uint64_t> intervals;
    for (size_t i = 0; i < EXPERIMENTS_NUM; i++) {
        auto t1 = std::chrono::high_resolution_clock::now();
        algo(dst, src, n);
        auto t2 = std::chrono::high_resolution_clock::now();
        uint64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
        intervals.push_back(duration);
    }
    uint64_t avrg_time = std::accumulate(intervals.begin(), intervals.end(), static_cast<uint64_t>(0))/EXPERIMENTS_NUM;

    /*std::for_each( intervals.begin(), intervals.end(),
                   [] (uint64_t val) { std::cout << val << std::endl; } );*/

    return avrg_time;
}

int main()
{
    uint64_t T = 0;
    uint16_t * src = new uint16_t[NUM];
    uint16_t * dst = new uint16_t[NUM];
    size_t n = NUM * sizeof(uint16_t);

    std::fill(src, src + NUM, 1);

    T = time(dst, src, n, memcpy_algo1);
    std::cout << "Algo1: Load and Store by 1 byte... " << T << " milliseconds" << std::endl;

    T = time(dst, src, n, memcpy_algo2);
    std::cout << "Algo2: Load and Store by 8 byte... " << T << " milliseconds" << std::endl;

    T = time(dst, src, n, memcpy_algo3);
    std::cout << "Algo3: Load and Store by 8 bytes + unroll 2x... " << T << " milliseconds" << std::endl;

    T = time(dst, src, n, memcpy_algo4);
    std::cout << "Algo4: Load and Store by 8 bytes + unroll 4x... " << T << " milliseconds" << std::endl;

    T = time(dst, src, n, memcpy_algo5);
    std::cout << "Algo5: Load and Store by 32 bytes... " << T << " milliseconds" << std::endl;

    T = time(dst, src, n, memcpy_algo6);
    std::cout << "Algo6: Load and Store by 32 bytes + unroll 2x... " << T << " milliseconds" << std::endl;

    T = time(dst, src, n, memcpy_algo7);
    std::cout << "Algo7: Load and Store by 1 byte run in parallel 8 threads... " << T << " milliseconds" << std::endl;

    T = time(dst, src, n, memcpy_opt_O3);
    std::cout << "Algo8: Load and Store by 1 byte optimized with -O3... " << T << " milliseconds" << std::endl;

    delete [] src;
    delete [] dst;

    return 0;
}