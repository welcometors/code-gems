// https://projecteuler.net/problem=63
/*
Powerful digit counts

The 5-digit number, 16807=7^5, is also a fifth power.
Similarly, the 9-digit number, 134217728=8^9, is a ninth power.

How many n-digit positive integers exist which are also an nth power?

Solution:
*/

#include <iostream>
#include <cstdint>
#include <cmath>
#include <chrono>

using namespace std;

auto compute() {
	int count = 1;		// 1^1
	for (int i = 9; i > 1; --i)
		for (int k = 1; ceil(k*log10(i)) >= k; ++k)
			++count;
	return count;
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