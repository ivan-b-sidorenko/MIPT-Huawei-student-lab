#include <iostream>
#include <vector>
#include <cstdio>
#include <ctime>

#include "../mul.hh"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

void usage( )
{
    std::cerr << "USAGE: ./PROGNAME [ALGNUM]" << endl;
    std::cerr << "0 - trivial, 1 - transpose" << endl;
}

using namespace MX;

using Func = Matrix<double> (*) (const Matrix<double> &, const Matrix<double> &);

auto speed_test( const Matrix<double> & m1, const Matrix<double> & m2, Func func )
{
    static uint counter = 0;

    cout << "Algo" << counter++ << ":" << endl;
    auto start = std::clock();

    Matrix<double> res = (*func)(m1, m2);
    auto time = static_cast<double>(std::clock() - start) /
                static_cast<double>(CLOCKS_PER_SEC);

    cout << time << " seconds" << endl;

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

    vector<Func> functions{MUL::trivial, MUL::transpose};

    for (auto && func : functions)
        speed_test(m1, m2, func);

    return 0;
}