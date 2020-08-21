// https://projecteuler.net/problem=70
/*
Totient permutation

Euler's Totient function, phi(n) [sometimes called the phi function], is used to
determine the number of numbers less than n which are relatively prime to n.
For example, as 1, 2, 4, 5, 7, and 8, are all less than nine and relatively prime to nine,
phi(9)=6.

The number 1 is considered to be relatively prime to every positive number, so phi(1)=1.

Interestingly, phi(87109)=79180, and it can be seen that 87109 is a permutation of 79180.

Find the value of n, 1 < n < 10^7, for which phi(n) is a permutation of n and the ratio 
n/phi(n) produces a minimum.

Solution:
*/

#include <iostream>
#include <cstdint>
#include <cmath>
#include <vector>
#include <memory>
#include <bitset>
#include <algorithm>
#include <chrono>

using namespace std;
typedef unsigned int natural;
constexpr natural limit = 10'000'000;
constexpr natural sqrtLimit = 3163;
constexpr natural searchLimit = 9 * sqrtLimit;
constexpr natural sqrtSearchLimit = 3 * sqrtLimit;

vector<int> primes;
bitset<searchLimit + 1> isPrime;
void populatePrimes() {
	isPrime.set();
	isPrime.reset(0);
	isPrime.reset(1);
	primes.clear();

	for (int i = 2; i <= sqrtSearchLimit; i++) {
		if (!isPrime.test(i))
			continue;
		primes.push_back(i);
		for (int j = i + i; j <= searchLimit; j += i)
			isPrime.reset(j);
	}

	for (int i = sqrtSearchLimit + 1; i <= searchLimit; i++) {
		if (isPrime.test(i))
			primes.push_back(i);
	}
}

bool isPermutation(natural a, natural b) {
	int dc[10] = {};
	while (a) {
		dc[a % 10]++;
		a /= 10;
	}
	while (b) {
		dc[b % 10]--;
		b /= 10;
	}
	for (auto v : dc)
		if (v)
			return false;
	return true;
}

unique_ptr<int[]> populatePhi(int limit) {
	unique_ptr<int[]> sieve(new int[limit + 1]);
	sieve[0] = 1;
	for (int i = 1; i <= limit; i++)
		sieve[i] = i;

	for (int i = 2; i <= limit; i++)
		if (sieve[i] == i)
			for (int j = i; j <= limit; j += i)
				sieve[j] -= sieve[j] / i;

	return sieve;
}

auto computeBF() {
	auto totients = populatePhi(limit);
	size_t minn = 0;
	double minr = 1e6;
	for (size_t n = 2; n < limit; ++n)
		if (isPermutation(n, totients[n])) {
			const double ratio = (double)n / totients[n];
			if (ratio < minr) {
				minn = n;
				minr = ratio;
			}
		}
	return minn;
}

auto compute() {
	populatePrimes();
	double maxInverseRatio = 0;
	natural maxn = 0;
	for (natural i = 1, size = primes.size(); i < size; i++)
		for (natural j = 0, n; j < i && (n = primes[i] * primes[j]) < limit; j++) {
			natural t = n - primes[j] - primes[i] + 1;
			if (isPermutation(n, t)) {
				const double ratio = (double)t / n;
				if (ratio > maxInverseRatio) {
					maxn = n;
					maxInverseRatio = ratio;
				}
			}
		}
	return maxn;
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