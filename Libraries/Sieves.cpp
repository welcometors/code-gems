#include <cstdint>
#include <cstring>
#include <cmath>

namespace Sieves {
	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

	constexpr size_t sizeOfElementInBits = 8 * sizeof(uint32_t);
	constexpr size_t logSizeOfElementInBits = compileTimeLogBase2(sizeOfElementInBits);

	class PrimeSieve {
		uint32_t *m_sieve;
	public:
		PrimeSieve(size_t limit) {
			const size_t numOfElements = (limit + sizeOfElementInBits) / sizeOfElementInBits;
			m_sieve = new uint32_t[numOfElements];
			std::memset(m_sieve, 0xAA, sizeof(uint32_t)*numOfElements);
			m_sieve[0] ^= 6;

			const size_t sqrtLimit = std::sqrt(limit + 1);
			for (size_t i = 3; i <= sqrtLimit; i += 2)
				if (m_sieve[i >> logSizeOfElementInBits] & (1 << (i&(sizeOfElementInBits - 1))))
					for (size_t j = i + i; j <= limit; j += i)
						m_sieve[j >> logSizeOfElementInBits] &= ~(1 << (j&(sizeOfElementInBits - 1)));
		}

		~PrimeSieve() {
			delete[] m_sieve;
		}

		inline bool isPrime(size_t number) const {
			return m_sieve[number >> logSizeOfElementInBits] & (1 << (number&(sizeOfElementInBits - 1)));
		}
	};
}