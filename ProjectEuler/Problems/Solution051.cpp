// https://projecteuler.net/problem=51
/*
Prime digit replacements

By replacing the 1st digit of the 2-digit number *3, it turns out that six
of the nine possible values: 13, 23, 43, 53, 73, and 83, are all prime.

By replacing the 3rd and 4th digits of 56**3 with the same digit, this
5-digit number is the first example having seven primes among the ten
generated numbers, yielding the family: 56003, 56113, 56333, 56443, 56663,
56773, and 56993. Consequently 56003, being the first member of this family,
is the smallest prime with this property.

Find the smallest prime which, by replacing part of the number (not
necessarily adjacent digits) with the same digit, is part of an eight
prime value family.

Solution:
Generate all primes of the given digits.
Generate all masks
Hash primes by mask
Select minimum
*/

#include <iostream>
#include <cstdint>
#include <cmath>
#include <memory>
#include <cstring>
#include <vector>
#include <unordered_map>
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

template<class T, size_t Base = 10>
inline T maskInt(T value, T mask) {
	// return value in base+1 for extra '?' digit
	T maskedValue = 0, power = 1, saved = Base + 1;
	while (value) {
		const T digit = value % Base;
		if (mask & 1)
			maskedValue += digit*power;
		else {
			if (saved != digit) {
				if (saved == Base + 1)
					saved = digit;
				else
					return -1;
			}
			maskedValue += Base*power;
		}
		value /= Base;
		power *= Base + 1;
		mask >>= 1;
	}
	return maskedValue;
}

template<class T, size_t Base = 10>
inline T unmaskInt(T family, T digit) {
	T unmaskedValue = 0, power = 1;
	while (family) {
		const T place = family % (Base + 1);
		unmaskedValue += (place < Base ? place : digit)*power;
		family /= Base + 1;
		power *= Base;
	}
	return unmaskedValue;
}

Sieves::PrimeSieve sieve(1'000'000);
using natural = uint32_t;

natural getMin(natural n, natural k, natural l) {
	if (n == k)
		return n == 2 ? 11 : -1;

	const natural limit = pow(10, n);
	if (l == 1)
		return limit + (n / 2) + 7 * (n / 4) - 2 * (n / 5) - 5 * (n / 6);

	vector<natural> primes;
	for (natural x = limit / 10 + 1; x < limit; x += 2)
		if (sieve.isPrime(x))
			primes.push_back(x);

	vector<natural> minSolution(l, limit);
	vector<bool> selected(n, false);
	fill(selected.begin(), selected.begin() + k, true);

	do {
		if (l > 4 && selected[0])
			continue;   // last digint can only have 1,3,7,9

		natural mask = ~0;
		for (natural i = 0; i < n; ++i)
			if (selected[i])
				mask ^= 1 << i;

		unordered_map<natural, natural> group;
		for (const auto& prime : primes) {
			const auto maskedPrime = maskInt(prime, mask);
			if (maskedPrime != -1)
				++group[maskedPrime];
		}

		for (const auto& family : group) {
			if (family.second >= l) {
				bool flag = false;
				vector<natural> solution;
				for (natural digit = (~mask) >> (n - 1), idx = 0, prime; digit < 10; ++digit) {
					if (sieve.isPrime(prime = unmaskInt(family.first, digit))) {
						if (flag || prime < minSolution[idx]) {
							solution.push_back(prime);
							if (solution.size() == l)
								break;
							flag = true;
						}
						else if (prime == minSolution[idx])
							solution.push_back(prime);
						else
							break;
					}
				}
				if (solution.size() == l)
					minSolution = move(solution);
			}
		}
	} while (prev_permutation(selected.begin(), selected.end()));

	/*for (const auto& prime : minSolution)
	cout << prime << ' ';*/
	return minSolution.front();
}

natural compute() {
	natural min = pow(10, 6);
	for (natural i = 1, prime; i < 5; ++i)
		if ((prime = getMin(6, i, 8)) < min)
			min = prime;
	return min;
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	auto end = high_resolution_clock::now();
	cout << result << '\n';
	cout << "Done in "
		<< duration_cast<nanoseconds>(end - start).count() / 1000000.0
		<< " miliseconds." << '\n';
}
