// https://projecteuler.net/problem=50
/*
Consecutive prime sum

The prime 41, can be written as the sum of six consecutive primes:

41 = 2 + 3 + 5 + 7 + 11 + 13
This is the longest sum of consecutive primes that adds to a prime below one-hundred.

The longest sum of consecutive primes below one-thousand that adds to a prime,
contains 21 terms, and is equal to 953.

Which prime, below one-million, can be written as the sum of the most consecutive primes?

Solution:

*/

#include <iostream>
#include <cstdint>
#include <cmath>
#include <tuple>
#include <memory>
#include <cstring>
#include <chrono>
using namespace std;

namespace ForisekJancina32 {
	// ref  Fast Primality Testing for Integers That Fit into a Machine Word
	uint16_t bases[] = { 15591,2018,166,7429,8064,16045,10503,4399,1949,1295,2776,3620,560,3128,5212,
		2657,2300,2021,4652,1471,9336,4018,2398,20462,10277,8028,2213,6219,620,3763,4852,5012,3185,
		1333,6227,5298,1074,2391,5113,7061,803,1269,3875,422,751,580,4729,10239,746,2951,556,2206,
		3778,481,1522,3476,481,2487,3266,5633,488,3373,6441,3344,17,15105,1490,4154,2036,1882,1813,
		467,3307,14042,6371,658,1005,903,737,1887,7447,1888,2848,1784,7559,3400,951,13969,4304,177,41,
		19875,3110,13221,8726,571,7043,6943,1199,352,6435,165,1169,3315,978,233,3003,2562,2994,10587,
		10030,2377,1902,5354,4447,1555,263,27027,2283,305,669,1912,601,6186,429,1930,14873,1784,1661,
		524,3577,236,2360,6146,2850,55637,1753,4178,8466,222,2579,2743,2031,2226,2276,374,2132,813,
		23788,1610,4422,5159,1725,3597,3366,14336,579,165,1375,10018,12616,9816,1371,536,1867,10864,
		857,2206,5788,434,8085,17618,727,3639,1595,4944,2129,2029,8195,8344,6232,9183,8126,1870,3296,
		7455,8947,25017,541,19115,368,566,5674,411,522,1027,8215,2050,6544,10049,614,774,2333,3007,
		35201,4706,1152,1785,1028,1540,3743,493,4474,2521,26845,8354,864,18915,5465,2447,42,4511,
		1660,166,1249,6259,2553,304,272,7286,73,6554,899,2816,5197,13330,7054,2818,3199,811,922,350,
		7514,4452,3449,2663,4708,418,1621,1171,3471,88,11345,412,1559,194 };

	bool is_SPRP(uint32_t n, uint32_t a) {
		uint32_t d = n - 1, s = 0;
		while ((d & 1) == 0) ++s, d >>= 1;
		uint64_t cur = 1, pw = d;
		while (pw) {
			if (pw & 1) cur = (cur*a) % n;
			a = ((uint64_t)a*a) % n;
			pw >>= 1;
		}
		if (cur == 1) return true;
		for (uint32_t r = 0; r<s; r++) {
			if (cur == n - 1) return true;
			cur = (cur*cur) % n;
		}
		return false;
	}

	bool isPrime(uint32_t x) {
		if (x == 2 || x == 3 || x == 5 || x == 7) return true;
		if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0) return false;
		if (x<121)
			return x > 1;

		uint64_t h = x;
		h = ((h >> 16) ^ h) * 0x45d9f3b;
		h = ((h >> 16) ^ h) * 0x45d9f3b;
		h = ((h >> 16) ^ h) & 255;
		return is_SPRP(x, bases[h]);
	}
}

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

			const size_t sqrtLimit = std::sqrt(minSieveSize);
			for (size_t i = 3; i <= sqrtLimit; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1))))
					for (size_t j = i + i; j <= minSieveSize; j += i)
						sieve[j >> logSizeOfElementInBits] &= ~(1 << (j&(sizeOfElementInBits - 1)));
		}

		inline bool isPrime(size_t number) const {
			return m_sieve[number >> logSizeOfElementInBits] & (1 << (number&(sizeOfElementInBits - 1)));
		}

		inline tuple<uint64_t, uint64_t, uint64_t> getSum(uint64_t n) {
			if (n <= 2)
				return make_tuple(0, 0, 0);

			uint32_t* const sieve = m_sieve.get();
			uint64_t sum = 2, last = 2, length = 1;
			for (size_t i = 3; sum + i < n; i += 2)
				if (sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1)))) {
					sum += i;
					last = i;
					++length;
				}
			return make_tuple(sum, length, last);
		}

		inline size_t nextPrime(size_t i) const {
			do {
				i += 2;
			} while (!(m_sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1)))));
			return i;
		}

		inline size_t prevPrime(size_t i) const {
			do {
				i -= 2;
			} while (!(m_sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1)))));
			return i;
		}
	};
}

using natural = uint32_t;
tuple<natural, natural, pair<natural, natural>> getMax(natural limit) {
	using namespace ForisekJancina32;

	// sum of primes upto 3931 is 10^6
	Sieves::PrimeSieve sieve(5000);

	natural fullSum, end, size;
	tie(fullSum, size, end) = sieve.getSum(limit);
	//cout << fullSum << ',' << size << ',' << end << '\n';

	while (size) {
		if (isPrime(fullSum))
			return make_tuple(fullSum, size, make_pair(2, end));

		natural last = sieve.nextPrime(end);
		natural sum = fullSum + last - 2;

		if (sum < limit) {
			if (isPrime(sum))
				return make_tuple(sum, size, make_pair(3, end));

			for (natural start = 3; ; ) {
				last = sieve.nextPrime(last);
				sum += last - start;
				if (sum >= limit)
					break;
				if (isPrime(sum))
					return make_tuple(sum, size, make_pair(start, end));
				start = sieve.nextPrime(start);
			}
		}

		--size;
		fullSum -= end;
		end = sieve.prevPrime(end);
	}

	return make_tuple(0, 0, make_pair(0, 0));
}

natural compute() {
	constexpr natural limit = 1000000;
	auto result = getMax(limit);
	//cout << get<0>(result) << ',' << get<1>(result) << ':' << get<2>(result).first << '-' << get<2>(result).second << '\n';
	return get<0>(result);
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

