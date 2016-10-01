// https://projecteuler.net/problem=49
/*
Prime permutations

The arithmetic sequence, 1487, 4817, 8147, in which each of the terms increases by 3330,
is unusual in two ways:
(i) each of the three terms are prime, and,
(ii) each of the 4-digit numbers are permutations of one another.

There are no arithmetic sequences made up of three 1-, 2-, or 3-digit primes,
exhibiting this property, but there is one other 4-digit increasing sequence.

What 12-digit number do you form by concatenating the three terms in this sequence?

Solution:

*/

#include <iostream>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
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

	std::string getDigitDistribution(size_t n) {
		char freq[11] = {};
		for (size_t i = 0; i < 10; i++)
			freq[i] = '0';

		while (n) {
			freq[n % 10]++;
			n /= 10;
		}
		return freq;
	}

	class PrimePermutationMap {
		std::unordered_map<std::string, std::vector<uint32_t>> primePerm;
	public:
		PrimePermutationMap(const size_t minSieveSize) {
			std::unique_ptr<uint32_t[]> m_sieve;
			const size_t numOfElementsInArray = 1 + minSieveSize / sizeOfElementInBits;
			m_sieve = std::make_unique<uint32_t[]>(numOfElementsInArray);
			const size_t limit = numOfElementsInArray*sizeOfElementInBytes;

			uint32_t* const sieve = m_sieve.get();
			std::memset(sieve, 0xAA, limit);
			sieve[0] ^= 6;
			primePerm.emplace(getDigitDistribution(2), std::vector<uint32_t>({ 2 }));

			const size_t sqrtLimit = std::sqrt(numOfElementsInArray*sizeOfElementInBits);
			for (size_t i = 3; i <= sqrtLimit; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1)))) {
					auto dst = getDigitDistribution(i);
					auto res = primePerm.find(dst);
					if (res != primePerm.end())
						res->second.push_back(i);
					else
						primePerm.emplace(dst, std::vector<uint32_t>({ i }));
					for (size_t j = i + i; j <= minSieveSize; j += i)
						sieve[j >> logSizeOfElementInBits] &= ~(1 << (j&(sizeOfElementInBits - 1)));
				}

			for (size_t i = (((sqrtLimit + 1) >> 1) << 1) | 1; i <= minSieveSize; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1)))) {
					auto dst = getDigitDistribution(i);
					auto res = primePerm.find(dst);
					if (res != primePerm.end())
						res->second.push_back(i);
					else
						primePerm.emplace(dst, std::vector<uint32_t>({ i }));
				}
		}

		inline std::unordered_map<std::string, std::vector<uint32_t>>& get() {
			return primePerm;
		}
	};
}

bool containsAP(const vector<uint32_t>& ary, size_t ans[3], size_t mid = 1) {
	const size_t n = ary.size();
	for (size_t m = mid; m < n - 1; m++) {
		size_t s = m - 1, e = m + 1;
		while (s >= 0 && e <= n - 1) {
			size_t sum = ary[s] + ary[e];
			if (sum == 2 * ary[m]) {
				ans[0] = s;
				ans[1] = m;
				ans[2] = e;
				return true;
			}
			if (sum < 2 * ary[m])
				e++;
			else if (s == 0)
				break;
			else
				s--;
		}
	}

	return false;
}

void printAllAPs(const vector<uint32_t>& ary) {
	const size_t n = ary.size();
	for (size_t s = 0; s < n - 2; ++s) {
		for (size_t m = s + 1; m < n - 1; ++m) {
			size_t next = 2 * ary[m] - ary[s];
			for (size_t e = m + 1; e < n; ++e)
				if (ary[e] == next)
					cout << ary[s] << ',' << ary[m] << ',' << ary[e] << '\n';
				else if (ary[e] > next)
					break;
		}
	}
}

uint64_t compute() {
	using natural = uint32_t;
	constexpr natural limit = 9999;
	Sieves::PrimePermutationMap ppm(limit);
	uint32_t series[3];

	for (const auto& data : ppm.get()) {
		const size_t n = data.second.size();
		if (n >= 3) {
			size_t mid = 1;
			while (containsAP(data.second, series, mid)) {
				if (data.second[series[0]] != 1487)
					return ((data.second[series[0]] * 10000ULL) + data.second[series[1]]) * 10000 + data.second[series[2]];
				mid = series[1] + 1;
			}
		}
	}
	return 0;
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
