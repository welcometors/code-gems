// https://projecteuler.net/problem=36
/*
Double-base palindromes

The decimal number, 585 = 10010010012 (binary), is palindromic in both bases.

Find the sum of all numbers, less than one million, which are palindromic in base 10 and base 2.

(Please note that the palindromic number, in either base, may not include leading zeros.)

Solution:

*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

bool isPalindromeBase2(unsigned n) {
	unsigned r = 0, x = n;
	while (x) {
		r = (r << 1) | (x & 1);
		x >>= 1;
	}
	return r == n;
}

unsigned check(unsigned n, bool both = true) {
	unsigned n1 = n, r = n / 10;
	while (r) {
		n1 = n1 * 10 + r % 10;
		r /= 10;
	}
	unsigned sum = isPalindromeBase2(n1) ? n1 : 0;
	if (both) {
		n1 = r = n;
		while (r) {
			n1 = n1 * 10 + r % 10;
			r /= 10;
		}
	}
	sum += isPalindromeBase2(n1) ? n1 : 0;
	return sum;
}

int compute() {
	int sum = 0;
	for (int i = 1; i < 1000; i++)
		sum += check(i);
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