// https://projecteuler.net/problem=75
/*
It turns out that 12 cm is the smallest length of wire that can be bent to form an
integer sided right angle triangle in exactly one way, but there are many more examples.

12 cm: (3,4,5)
24 cm: (6,8,10)
30 cm: (5,12,13)
36 cm: (9,12,15)
40 cm: (8,15,17)
48 cm: (12,16,20)

In contrast, some lengths of wire, like 20 cm, cannot be bent to form an integer
sided right angle triangle, and other lengths allow more than one solution to be found;
for example, using 120 cm it is possible to form exactly three different integer sided
right angle triangles.

120 cm: (30,40,50), (20,48,52), (24,45,51)

Given that L is the length of the wire, for how many values of L <= 1,500,000 can exactly
one integer sided right angle triangle be formed?
Solution:
*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <memory>
#include <cstring>
#include <chrono>
using namespace std;

template<class T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type GCD(T u, T v) {
	int shift;
	if (u == 0) return v;
	if (v == 0) return u;

	for (shift = 0; ((u | v) & 1) == 0; ++shift) {
		u >>= 1;
		v >>= 1;
	}

	while ((u & 1) == 0)
		u >>= 1;

	do {
		while ((v & 1) == 0)
			v >>= 1;
		if (u > v) {
			T t = v;
			v = u;
			u = t;
		}
		v = v - u;
	} while (v != 0);

	return u << shift;
}

namespace BitArray {
	using baseType = uint32_t;
	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

	constexpr size_t sizeOfElementInBytes = sizeof(baseType);
	constexpr size_t sizeOfElementInBits = 8 * sizeOfElementInBytes;
	constexpr size_t logSizeOfElementInBits = compileTimeLogBase2(sizeOfElementInBits);

	class Bitset {
		std::unique_ptr<baseType[]> m_array;
		size_t m_numOfElementsInArray;
	public:
		Bitset(const Bitset&) = default;
		Bitset(Bitset&&) = default;
		Bitset& operator=(const Bitset&) = default;
		Bitset& operator=(Bitset&&) = default;

		explicit Bitset(const size_t size) : m_numOfElementsInArray(1 + size / sizeOfElementInBits) {
			m_array = std::make_unique<baseType[]>(m_numOfElementsInArray);
			clear();
		}

		inline void clear() {
			std::memset(m_array.get(), 0x00, m_numOfElementsInArray*sizeOfElementInBytes);
		}

		inline void set() {
			std::memset(m_array.get(), 0xFF, m_numOfElementsInArray*sizeOfElementInBytes);
		}

		inline void clear(size_t idx) {
			m_array[idx >> logSizeOfElementInBits] &= ~(1 << (idx&(sizeOfElementInBits - 1)));
		}

		inline void set(size_t idx) {
			m_array[idx >> logSizeOfElementInBits] |= 1 << (idx&(sizeOfElementInBits - 1));
		}

		inline bool test(size_t idx) const {
			return m_array[idx >> logSizeOfElementInBits] & (1 << (idx&(sizeOfElementInBits - 1)));
		}

		auto count() {
			baseType sum = 0;
			for (size_t i = 0; i < m_numOfElementsInArray; ++i)
#ifdef _MSC_VER
				sum += _mm_popcnt_u32(m_array[i]);
#else
				sum += __builtin_popcount(m_array[i]);
#endif		
			return sum;
		}
	};
}

using u32 = unsigned;
auto compute() {
	const u32 s = 1'500'000;
	BitArray::Bitset appeared(s), moreThan1(s);
	u32 s2 = s / 2;
	u32 lim = ceil(sqrt(s*.5)) - 1;
	for (u32 m = 2; m <= lim; m++) {
		for (u32 n = 1; n < m; n++) {
			if (GCD(m, n) == 1) {
				u32 a = m*m - n*n, b = 2 * m*n, c = m*m + n*n;
				if (GCD(a, b) == 1) {
					if (s2 < c)
						break;
					for (u32 r = 1, k = a + b + c; r <= s / c && r*k <= s; r++) {
						if (appeared.test(r*k))
							moreThan1.set(r*k);
						else
							appeared.set(r*k);
						//printf("%4d: %d^2 x %d^2 = %d^2\n", r*(a + b + c), r*a, r*b, r*c);
					}
				}
			}
		}
	}

	u32 count = 0;
	for (u32 i = 0; i <= s; ++i)
		if (appeared.test(i) && !moreThan1.test(i))
			++count;
	return count;
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