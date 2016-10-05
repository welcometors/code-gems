// https://projecteuler.net/problem=41
/*
Pandigital prime

We shall say that an n-digit number is pandigital if it makes use of all the 
digits 1 to n exactly once. For example, 2143 is a 4-digit pandigital and 
is also prime.

What is the largest n-digit pandigital prime that exists?

Solution:

*/

#include <iostream>
#include <cmath>
#include <memory>
#include <cstring>
#include <vector>
#include <algorithm>
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
		explicit PrimeSieve(const size_t minSieveSize) {
			const size_t numOfElementsInArray = 1 + minSieveSize / sizeOfElementInBits;
			m_sieve = std::make_unique<uint32_t[]>(numOfElementsInArray);
			const size_t limit = numOfElementsInArray*sizeOfElementInBytes;

			uint32_t* const sieve = m_sieve.get();
			std::memset(sieve, 0xAA, limit);
			sieve[0] ^= 6;

			const size_t sqrtLimit = std::sqrt(numOfElementsInArray*sizeOfElementInBits);
			for (size_t i = 3; i <= sqrtLimit; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1))))
					for (size_t j = i + i; j <= minSieveSize; j += i)
						sieve[j >> logSizeOfElementInBits] &= ~(1 << (j&(sizeOfElementInBits - 1)));
		}

		inline bool isPrime(size_t number) const {
			return m_sieve[number >> logSizeOfElementInBits] & (1 << (number&(sizeOfElementInBits - 1)));
		}
	};
}

unsigned checkAndGet(const Sieves::PrimeSieve& sieve, unsigned n) {
	vector<unsigned> digits;
	for (unsigned i = 1; i <= n; ++i)
		digits.push_back(i);

	unsigned max = 0;
	do {
		unsigned val = 0;
		for (auto d : digits)
			val = val * 10 + d;
		if (sieve.isPrime(val) && val > max)
			max = val;
	} while (next_permutation(digits.begin(), digits.end()));
	return max;
}

unsigned compute() {
	constexpr int limit = 7654321;
	Sieves::PrimeSieve sieve(limit + 1);
	return checkAndGet(sieve, 7);
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
