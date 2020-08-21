// https://projecteuler.net/problem=97
/*
Problem 97

Large non-Mersenne prime

The first known prime found to exceed one million digits was discovered in 1999,
and is a Mersenne prime of the form 2^6972593-1;
it contains exactly 2,098,960 digits.
Subsequently other Mersenne primes, of the form 2^p-1, have been found which
contain more digits.

However, in 2004 there was found a massive non-Mersenne prime which contains
2,357,207 digits: 28433*2^7830457+1.

Find the last ten digits of this prime number.

Solution:
*/

#include <iostream>
#include <chrono>
using namespace std;
using natural = uint64_t;

natural mulMod64(natural a, natural b, natural m) {
    int64_t res = 0;
    while (a != 0) {
        if (a & 1) res = (res + b) % m;
        a >>= 1;
        b = (b << 1) % m;
    }
    return res;
}

natural modPow64(natural i, natural j, natural m) {
    if (!j)
        return 1;

    if (i < 2)
        return i;

    natural p = 1;
    while (j) {
        if (j & 1)
            p = mulMod64(p, i, m);
        i = mulMod64(i, i, m);
        j >>= 1;
    }

    return p;
}

// computes (A*B^C+D) % E
natural calc64(natural a, natural b, natural c, natural d, natural e) {
    return (mulMod64(a, modPow64(b, c, e), e) + d) % e;
}

auto compute() {
    return calc64(28433, 2, 7830457, 1, 10'000'000'000);
}

template<typename Function, class ... Types>
decltype(auto) timeit(Function f, Types ... args) {
    using namespace chrono;
    auto start = high_resolution_clock::now();
    auto result = f(args...);
    double duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6;
    return std::make_pair(result, duration);
}

int main() {
    using namespace std;
    auto[result, time] = timeit(compute);
    cout << result << " Calculated in " << time << " miliseconds." << '\n';
    return 0;
}