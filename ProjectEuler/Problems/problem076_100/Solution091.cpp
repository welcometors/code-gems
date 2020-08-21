// https://projecteuler.net/problem=91
/*
Problem 91

Right triangles with integer coordinates

The points P (x1, y1) and Q (x2, y2) are plotted at integer co-ordinates and are joined
to the origin, O(0,0), to form triangle OPQ.

There are exactly fourteen triangles containing a right angle that can be formed when
each co-ordinate lies between 0 and 2 inclusive; that is, 0 <= x1, y1, x2, y2 <= 2.

Given that 0 <= x1, y1, x2, y2 <= 50, how many right triangles can be formed?

Solution:
*/

#include <iostream>
#include <cmath>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

constexpr double epsilon = .000001;

bool check(double a, double b, double c) {
	if (a > b && a > c && a < b + c)
		return abs(a*a - b*b - c*c) < epsilon;
	if (b > a && b > c && b < a + c)
		return abs(b*b - a*a - c*c) < epsilon;
	if (c > a && c > b && c < a + b)
		return abs(c*c - a*a - b*b) < epsilon;
	return false;
}

unsigned calcV1(int n) {
	unsigned count = 0;

	for (int x1 = 0; x1 <= n; x1++) {	// Ax
		for (int y1 = 0; y1 <= n; y1++) {	// Ay
			for (int x2 = 0; x2 <= n; x2++) {	// Bx
				for (int y2 = 0; y2 <= n; y2++) {	// By
					double oa = sqrt(x1*x1 + y1*y1);
					double ob = sqrt(x2*x2 + y2*y2);
					double ab = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
					if (check(oa, ob, ab)) {
						//if (abs(oa*oa - ob*ob - ab*ab) < epsilon && y2 != 0)
						//printf("%f %f %f : %d,%d %d,%d\n", oa, ob, ab, x1, y1, x2, y2);
						count++;
					}
				}
			}
		}
	}

	return count / 2;
}

template<class T>
T GCD(T u, T v) {
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

unsigned calcV2(unsigned n) {
	unsigned c = 0;
	for (unsigned y = 1; y <= n; y++) {
		c += min(n - y, y);
		for (unsigned x = y + 1; x <= n; x++) {
			// slope of OB = y/x
			// now slope of BA = -x/y
			const unsigned z = GCD(x, y);
			c += min((n - y) / (x / z), x / (y / z));
			// repeat for point(y,x) as gcd is same
			c += min((n - x) / (y / z), y / (x / z));
		}
	}
	return 2 * c + n*n * 3;
}

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

auto calcV3(size_t n) {
	// using sieve to avoid calculating GCD
	BitArray::bitarray visited((n + 1)*(n + 1));
	uint64_t c = 0;
	for (size_t y = 1, yo = n + 1; y <= n / 2; y++, yo += n + 1) {
		for (size_t x = 1; x <= n / 2; x++) {
			if (!visited.get(yo + x)) {
				for (size_t x1 = x, y1 = y; x1 <= n && y1 <= n; x1 += x, y1 += y) {
					if (x1*x1 < y1*(n - y1)) // hit the left edge
						c += x1 / y;
					else
						c += (n - y1) / x;
					visited.set(y1*(n + 1) + x1);
				}
			}
		}
		// count remaining
		const size_t a = n / 2 + 1, b = n - y;
		if (a <= b)
			c += b - a + 1 - visited.count(yo + a, yo + b);
	}
	return 2 * c + n*n * 3;
}

auto compute() {
	return calcV3(50);
}

template<typename Function, class ... Types>
decltype(auto) timeit(Function f, Types ... args) {
    using namespace chrono;
    auto start = high_resolution_clock::now();
    auto result = f(args...);
    double duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6;
    return std::make_pair(result, duration);
}

int main() {
    using namespace std;
    auto[result, time] = timeit(compute);
    cout << result << " Calculated in " << time << " miliseconds." << '\n';
    return 0;
}