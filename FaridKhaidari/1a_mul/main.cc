#include <iostream>
#include <vector>
#include <cstdio>
#include <chrono>

#include "mul.hh"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

using namespace MXL;

using Func = Matrix<double> (*) (const Matrix<double> &, const Matrix<double> &);

auto speed_test( const Matrix<double> & m1,
                 const Matrix<double> & m2,
                 const Matrix<double> & mul,
                 Func func )
{
    static uint counter = 0;

    cout << "Algo" << counter++ << ": ";
    auto start = std::chrono::steady_clock::now();

    Matrix<double> res = (*func)(m1, m2);
    auto end = std::chrono::steady_clock::now();
    
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    if (mul != res)
        cout << "INCORRECT" << endl;

    cout << time << " milliseconds" << endl << endl;

    return res;
}

int main( )
{
    uint rows1{}, rows2{},
         cols1{}, cols2{};

    cin >> rows1 >> cols1 >> rows2 >> cols2;

    vector<double> raw1{}, raw2{};

    raw1.reserve(rows1 * cols1);
    
    for (int i = 0, end = rows1 * cols1; i < end; ++i)
    {
        double tmp;
        cin >> tmp;
        raw1.push_back(tmp);
    }

    raw2.reserve(rows2 * cols2);
    for (int i = 0, end = rows2 * cols2; i < end; ++i)
    {
        double tmp;
        cin >> tmp;
        raw2.push_back(tmp);
    }

    Matrix<double> m1{rows1, cols1, raw1.begin(), raw1.end()};
    Matrix<double> m2{rows2, cols2, raw2.begin(), raw2.end()};

    assert(cols1 == rows2);
    Matrix<long double> res{rows1, cols2};

    vector<Func> functions{MUL::trivial, 
                           MUL::transpose,

                           MUL::trivial_cycle2x,
                           MUL::transpose_cycle2x,

                           MUL::trivial_cycle4x,
                           MUL::transpose_cycle4x,

                           MUL::trivial_cycle8x,
                           MUL::transpose_cycle8x,
                           
                           MUL::trivial_threads,
                           MUL::transpose_threads,
                           
                           MUL::trivial_threads2x};

    auto mul = m1 * m2;
    for (auto && func : functions)
        speed_test(m1, m2, mul, func);

    return 0;
}