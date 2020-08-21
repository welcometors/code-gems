// https://projecteuler.net/problem=65
/*
Convergents of e

The square root of 2 can be written as an infinite continued fraction.

2^1/2 = 1 + 1/(2 + 1/(2 + 1/(2 + ... ))) = 1.414213...

The infinite continued fraction can be written,
2^1/2 = [1;(2)],
where (2) indicates that 2 repeats ad infinitum.
In a similar way, 23^1/2 = [4;(1,3,1,8)].

Hence the sequence of the first ten convergents for 2^1/2 are:

1, 3/2, 7/5, 17/12, 41/29, 99/70, 239/169, 577/408, 1393/985, 3363/2378, ...
What is most surprising is that the important mathematical constant,
e = [2; 1,2,1, 1,4,1, 1,6,1 , ... , 1,2k,1, ...].

The first ten terms in the sequence of convergents for e are:

2, 3, 8/3, 11/4, 19/7, 87/32, 106/39, 193/71, 1264/465, 1457/536, ...
The sum of digits in the numerator of the 10th convergent is 1+4+5+7=17.

Find the sum of digits in the numerator of the 100th convergent of the
continued fraction for e.

Solution:
*/

#include <iostream>
#include <cstdint>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <chrono>

auto compute() {
	using namespace boost::multiprecision;
	constexpr uint32_t limit = 100;

	cpp_int n = 2, d = 1;
	for (uint32_t i = 2; i <= limit; ++i) {
		cpp_int c = i % 3 == 0 ? 2 * (i / 3) : 1;
		cpp_int t = d;
		d = n;
		n = c*d + t;
	}

	int sum = 0;
	for (char c : n.str())
		sum += c - '0';
	return sum;
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