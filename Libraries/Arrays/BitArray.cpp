#include <vector>

namespace BitArray {
	constexpr size_t compileTimeLogBase2(size_t n) {
		return (n <= 1) ? 0 : 1 + compileTimeLogBase2(n / 2);
	}

#ifdef _MSC_VER
#include <nmmintrin.h>
	uint32_t popcnt(uint32_t x) { return _mm_popcnt_u32(x); }
#else
	uint32_t popcnt(uint32_t x) { return __builtin_popcount(x); }
#endif

	using elementType = uint32_t;
	constexpr size_t elementSize = sizeof(elementType) * 8;
	constexpr size_t elementSize_1 = elementSize - 1;
	constexpr size_t lgElementSize = compileTimeLogBase2(elementSize);

	class bitarray {
	private:
		std::vector<elementType> data;
	public:
		explicit bitarray(size_t n) : data((n + elementSize_1) / elementSize) {}

		inline void clear() {
			std::fill(data.begin(), data.end(), elementType(0));
		}

		inline int get(size_t n) const {
			return data[n >> lgElementSize] & (elementType(1) << (n & elementSize_1));
		}

		inline void set(size_t n) {
			data[n >> lgElementSize] |= elementType(1) << (n & elementSize_1);
		}

		inline void toggle(size_t n) {
			data[n >> lgElementSize] ^= elementType(1) << (n & elementSize_1);
		}

		inline void reset(size_t n) {
			data[n >> lgElementSize] &= ~(elementType(1) << (n & elementSize_1));
		}

		inline uint32_t count() const {
			uint32_t c = 0;
			for (auto& x : data)
				c += popcnt(x);
			return c;
		}

		// given l <= r
		inline uint32_t count(size_t l, size_t r) const {
			const size_t ai = l >> lgElementSize;
			const size_t bi = r >> lgElementSize;

			uint32_t c = 0;
			if (ai == bi) {
				for (size_t i = l; i <= r; i++)
					if (get(i))
						c++;
			}
			else {
				for (size_t i = l; i < (ai + 1) << lgElementSize; i++)
					if (get(i))
						c++;
				for (size_t i = bi << lgElementSize; i <= r; i++)
					if (get(i))
						c++;
				for (size_t i = ai + 1; i < bi; i++)
					c += popcnt(data[i]);
			}
			return c;
		}
	};
}