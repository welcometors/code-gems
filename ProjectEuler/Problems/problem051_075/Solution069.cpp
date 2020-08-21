// https://projecteuler.net/problem=69
/*
Totient maximum

Euler's Totient function, phi(n) [sometimes called the phi function], is used to
determine the number of numbers less than n which are relatively prime to n.
For example, as 1, 2, 4, 5, 7, and 8, are all less than nine and relatively prime to nine,
phi(9)=6.

It can be seen that n=6 produces a maximum n/phi(n) for n <= 10.

Find the value of n <= 1,000,000 for which n/phi(n) is a maximum.

Solution:
*/

#include <iostream>
#include <cstdint>
#include <chrono>

auto compute() {
	constexpr uint32_t limit = 1'000'000;
	const uint32_t primes[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47 };

	uint32_t result = 1;
	for (const auto& prime : primes)
		if (result*prime <= limit)
			result *= prime;
		else
			break;
	return result;
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