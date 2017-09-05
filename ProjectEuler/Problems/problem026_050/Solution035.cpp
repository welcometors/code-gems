// https://projecteuler.net/problem=35
/*
Circular primes

The number, 197, is called a circular prime because all rotations of the digits: 197, 971, and 719, are themselves prime.

There are thirteen such primes below 100: 2, 3, 5, 7, 11, 13, 17, 31, 37, 71, 73, 79, and 97.

How many circular primes are there below one million?

Solution:

*/

#include <iostream>
#include <bitset>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

typedef unsigned long natural;

constexpr int limit = 1000000;
bitset<limit + 1> isPrime;
void populatePrimes() {
	int sqrtLimit = (int)sqrt(limit + 1) + 1;
	isPrime.set();
	isPrime.reset(0);
	isPrime.reset(1);

	for (int i = 2; i <= sqrtLimit; i++) {
		if (!isPrime.test(i))
			continue;
		for (int j = i + i; j <= limit; j += i)
			isPrime.reset(j);
	}
}

int checkAndGet(int n, int d) {
	if (!d--) {
		if (!isPrime.test(n))
			return 0;

		int c = 0, x = n, p = 1;
		while (x) {
			c++;
			p *= 10;
			x /= 10;
		}
		p /= 10;
		for (int i = 1; i < c; i++) {
			n = n / 10 + p*(n % 10);
			if (!isPrime.test(n))
				return 0;
		}
		//cout << n << endl;
		return 1;
	}

	n *= 10;
	int sum = 0;
	for (int i = 1; i < 10; i += 2)
		sum += checkAndGet(n + i, d);
	return sum;
}

int compute() {
	populatePrimes();

	int sum = 4; // 2, 3, 5, 7
	for (int i = 2; i <= 6; i++)
		sum += checkAndGet(0, i);
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