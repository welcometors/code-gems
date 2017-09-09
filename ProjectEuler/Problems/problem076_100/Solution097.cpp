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
using natural = unsigned long long;

uint64_t mulMod64(uint64_t a, uint64_t b, uint64_t m) {
	int64_t res = 0;
	while (a != 0) {
		if (a & 1) res = (res + b) % m;
		a >>= 1;
		b = (b << 1) % m;
	}
	return res;
}

uint64_t modPow64(uint64_t i, uint64_t j, uint64_t m) {
	if (!j)
		return 1;

	if (i < 2)
		return i;

	uint64_t p = 1;
	while (j) {
		if (j & 1)
			p = mulMod64(p, i, m);
		i = mulMod64(i, i, m);
		j >>= 1;
	}

	return p;
}

// computes (A*B^C+D) % E
uint64_t calc64(uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e) {
	return (mulMod64(a, modPow64(b, c, e), e) + d) % e;
}

auto compute() {
	return calc64(28433, 2, 7830457, 1, 10'000'000'000);
}

#ifdef _MSC_VER
	template <class T>
	inline void DoNotOptimize(const T &value) {
		__asm { lea ebx, value }
	}
#else
	template <class T>
	__attribute__((always_inline)) inline void DoNotOptimize(const T &value) {
		asm volatile("" : "+m"(const_cast<T &>(value)));
	}
#endif

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6
		<< " miliseconds." << endl;
	cout << result << endl;
}