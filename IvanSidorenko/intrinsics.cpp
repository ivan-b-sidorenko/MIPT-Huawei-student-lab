#include <immintrin.h>
#include <iostream>

#define SIZE 18

int dot_naive(void *a, void *b, size_t size) {
    int sum = 0;
    int * pA = static_cast<int*>(a);
    int * pB = static_cast<int*>(b);
    for (size_t i = 0; i < size; i++) {
        sum += pA[i] * pB[i];
    }
    return sum;
}

int dot_optimized(void *a, void *b, size_t size) {
    int block = sizeof(__m128i) / sizeof(int);
    __m128i * pA = static_cast<__m128i*>(a);
    __m128i * pB = static_cast<__m128i*>(b);
    __m128i val_c = _mm_setzero_si128();
    for (size_t i = 0; i < (size/block); i++) {
      __m128i val_a = _mm_load_si128(pA);
      __m128i val_b = _mm_load_si128(pB);
      __m128i mul = _mm_mullo_epi32(val_a, val_b);
      val_c = _mm_add_epi32(val_c, mul);
      pA++;
      pB++;
    }
    __m128i tmp = _mm_hadd_epi32(val_c, val_c);
    __m128i tmp2 = _mm_hadd_epi32(tmp, tmp);
    int body = _mm_cvtsi128_si32(tmp2);

    int * pAi = static_cast<int*>(a);
    int * pBi = static_cast<int*>(b);
    int tail = 0;
    for (size_t i = (size/block)*block; i < size; i++) {
        tail += pAi[i] * pBi[i];
    }

    return body + tail;
}

int main() {
    int * a = new int[SIZE];
    int * b = new int[SIZE];

    for (size_t i = 0; i < SIZE; i++) {
        a[i] = 1;
        b[i] = 2;
    }

    int result = dot_optimized(a, b, SIZE);

    std::cout << result << std::endl;

    delete [] a;
    delete [] b;

    return 0;
}