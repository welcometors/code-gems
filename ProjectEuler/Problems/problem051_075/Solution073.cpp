// https://projecteuler.net/problem=73
/*
Ordered fractions

Consider the fraction, n/d, where n and d are positive integers. If n<d and HCF(n,d)=1, it is called a reduced proper fraction.

If we list the set of reduced proper fractions for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that there are 3 fractions between 1/3 and 1/2.

How many fractions lie between 1/3 and 1/2 in the sorted set of reduced proper fractions for d <= 12,000?

Solution:
*/

#include <iostream>
#include <cmath>
#include <memory>
#include <cstring>
#include <vector>
#include <chrono>
using namespace std;

namespace Sieves {
	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

	constexpr size_t sizeOfElementInBytes = sizeof(uint32_t);
	constexpr size_t sizeOfElementInBits = 8 * sizeOfElementInBytes;
	constexpr size_t logSizeOfElementInBits = compileTimeLogBase2(sizeOfElementInBits);

	class PrimeSieve {
		std::unique_ptr<uint32_t[]> m_sieve;
	public:
		PrimeSieve(const PrimeSieve&) = default;
		PrimeSieve(PrimeSieve&&) = default;
		PrimeSieve& operator=(const PrimeSieve&) = default;
		PrimeSieve& operator=(PrimeSieve&&) = default;

		explicit PrimeSieve(const size_t minSieveSize) {
			const size_t numOfElementsInArray = 1 + minSieveSize / sizeOfElementInBits;
			m_sieve = std::make_unique<uint32_t[]>(numOfElementsInArray);
			const size_t totalSizeInBytes = numOfElementsInArray*sizeOfElementInBytes;

			uint32_t* const sieve = m_sieve.get();
			std::memset(sieve, 0xAA, totalSizeInBytes);
			sieve[0] ^= 6;

			const size_t limit = numOfElementsInArray*sizeOfElementInBits;
			const size_t sqrtLimit = std::sqrt(limit - 1);
			for (size_t i = 3; i <= sqrtLimit; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1))))
					for (size_t j = i + i; j < limit; j += i)
						sieve[j >> logSizeOfElementInBits] &= ~(1 << (j&(sizeOfElementInBits - 1)));
		}

		inline bool isPrime(size_t number) const {
			return m_sieve[number >> logSizeOfElementInBits] & (1 << (number&(sizeOfElementInBits - 1)));
		}
	};
}

using ull = unsigned long long;
vector<unsigned> primes;

inline unsigned primesBelowN(unsigned n) {
	return 1.25506 * n / log(n);	// valid for x >= 17
}

auto generatePrimes(unsigned n) {
	Sieves::PrimeSieve sieve(n);
	vector<unsigned> primes = { 2 };
	primes.reserve(primesBelowN(n));
	for (unsigned i = 3; i <= n; i += 2)
		if (sieve.isPrime(i))
			primes.push_back(i);
	return primes;
}

inline ull sumTillNbyA(ull n, ull a) {
	const ull q = n / a, r = n % a;
	if (q == 0)
		return 0;
	return (q*(q - 1) / 2)*a + q*(r + 1);
}

inline ull allTillN(ull n) {
	return sumTillNbyA(n - 1, 2) - sumTillNbyA(n, 3);
}

ull inclusionExclusion(ull limit, ull index) {
	ull count = allTillN(limit);
	for (; 5 * primes[index] <= limit; ++index)
		count -= inclusionExclusion(limit / primes[index], index + 1);
	return count;
}

auto compute() {
	constexpr ull limit = 12'000;
	primes = generatePrimes(limit);
	return inclusionExclusion(limit, 0);
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

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