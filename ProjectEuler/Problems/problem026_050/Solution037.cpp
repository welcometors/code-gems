// https://projecteuler.net/problem=37
/*
Truncatable primes

The number 3797 has an interesting property. Being prime itself, 
it is possible to continuously remove digits from left to right, 
and remain prime at each stage: 3797, 797, 97, and 7. 
Similarly we can work from right to left: 3797, 379, 37, and 3.

Find the sum of the only eleven primes that are both truncatable 
from left to right and right to left.

NOTE: 2, 3, 5, and 7 are not considered to be truncatable primes.

Solution:

*/

#include <iostream>
#include <cmath>
#include <bitset>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

// Since 'checkAndGet' will be called only 216 times we dont need sieve.
bool isPrime(unsigned n) {
	if (n == 2 || n == 3)
		return true;
	if (n < 2 || n % 2 == 0 || n % 3 == 0)
		return false;

	for (unsigned i = 5, l = sqrt(n); i <= l; i += 4) {
		if (n%i == 0)
			return false;
		i += 2;
		if (n%i == 0)
			return false;
	}
	return true;
}

unsigned checkAndGet(unsigned n, unsigned d) {
	if (!d--) {
		unsigned p = 1, t = n;
		while (t) {
			p *= 10;
			if (!isPrime(n%p))
				return 0;
			t /= 10;
		}
		//cout << n << endl;
		return n;
	}

	unsigned sum = 0;
	for (unsigned i = 1; i < 10; i += i < 3 ? 1 : 2)
		if (isPrime(n * 10 + i))
			sum += checkAndGet(n * 10 + i, d);
	return sum;
}

unsigned compute() {
	unsigned sum = 0;
	for (unsigned i = 2; i <= 6; i++)
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