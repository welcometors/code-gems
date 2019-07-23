#include <cstdint>
#include <cstring>
#include <memory>
#include <cmath>

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

		auto countPrimes(const size_t minSieveSize) {
			const size_t lastIdx = minSieveSize / sizeOfElementInBits;
			uint32_t sum = 0;
			for (size_t i = 0; i < lastIdx; ++i)
		#ifdef _MSC_VER
			    sum += _mm_popcnt_u32(m_sieve[i]);
		#else
			    sum += __builtin_popcount(m_sieve[i]);
		#endif		
			const size_t remaining = minSieveSize & (sizeOfElementInBits - 1);
			for (size_t i = 0; i < remaining; ++i)
			    if((m_sieve[lastIdx] >> i) & 1)
				sum++;
			return sum;
		}
	};

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
