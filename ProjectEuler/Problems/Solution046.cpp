// https://projecteuler.net/problem=46
/*
Goldbach's other conjecture

It was proposed by Christian Goldbach that every odd composite number
can be written as the sum of a prime and twice a square.

 9 =  7 + 2×12
15 =  7 + 2×22
21 =  3 + 2×32
25 =  7 + 2×32
27 = 19 + 2×22
33 = 31 + 2×12

It turns out that the conjecture was false.

What is the smallest odd composite that cannot be written as the sum of a
prime and twice a square?

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
	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

	constexpr size_t sizeOfElementInBytes = sizeof(uint32_t);
	constexpr size_t sizeOfElementInBits = 8 * sizeOfElementInBytes;
	constexpr size_t logSizeOfElementInBits = compileTimeLogBase2(sizeOfElementInBits);

	class PrimeSieve {
		std::unique_ptr<uint32_t[]> m_sieve;
	public:
		PrimeSieve(const size_t minSieveSize) {
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

using natural = uint32_t;
constexpr natural limit = 10000;
Sieves::PrimeSieve sieve(limit);

bool check(natural n) {
	for (natural ii2 = 2, d = 6; n > ii2; ii2 += d, d += 4)
		if (sieve.isPrime(n - ii2))
			return true;
	return false;
}

natural compute() {
	for (natural x = 9; ; x += 2)
		if (!sieve.isPrime(x) && !check(x))
			return x;
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
