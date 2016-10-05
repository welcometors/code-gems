// https://projecteuler.net/problem=47
/*
Distinct primes factors

The first two consecutive numbers to have two distinct prime factors are:

14 = 2 × 7
15 = 3 × 5

The first three consecutive numbers to have three distinct prime factors are:

644 = 2^2 ×  7 × 23
645 =   3 ×  5 × 43
646 =   2 × 17 × 19.

Find the first four consecutive integers to have four distinct prime factors.
What is the first of these numbers?

Solution:

*/

#include <iostream>
#include <cstdint>
#include <cstring>
#include <memory>
#include <cmath>
#include <chrono>
using namespace std;

namespace Sieves {
	class FactorCountSieve {
		std::unique_ptr<uint8_t[]> m_sieve;
	public:
		explicit FactorCountSieve(const size_t limit) : m_sieve(new uint8_t[limit + 1]) {
			uint8_t* const sieve = m_sieve.get();
			std::memset(sieve, 0, limit + 1);
			for (size_t i = 2; i <= limit; ++i)
				if (!sieve[i])
					for (size_t j = i + i; j <= limit; j += i)
						sieve[j]++;
		}

		inline uint8_t numOfFactors(size_t number) const {
			return m_sieve[number];
		}

		inline uint8_t* get() const {
			return m_sieve.get();
		}
	};
}

size_t compute() {
	constexpr size_t limit = 200000;
	Sieves::FactorCountSieve sieve(limit);
	uint8_t* factors = sieve.get();

	constexpr uint8_t distinctFactors = 4;
	for (size_t x = 2, c = 0; x <= limit; ++x)
		if (factors[x] == distinctFactors) {
			c++;
			if (c == distinctFactors)
				return x - distinctFactors + 1;
		}
		else
			c = 0;
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	auto end = high_resolution_clock::now();
	cout << result << '\n';
	cout << "Done in "
		<< duration_cast<nanoseconds>(end - start).count() / 1000000.0
		<< " miliseconds." << '\n';
}
