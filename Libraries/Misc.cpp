template <typename T>
bool isPalindrome(T n){
	T r = 0, c = n;
	while (c){
		r = r * 10 + c % 10;
		c /= 10;
	}
	return r == n;
}

unsigned long isqrt(unsigned long x){
	register unsigned long op, res, one;
	op = x;
	res = 0;
	/* "one" starts at the highest power of four <= than the argument. */
	one = 1 << 30;  /* second-to-top bit set */
	while (one > op)
		one >>= 2;

	while (one != 0){
		if (op >= res + one){
			op -= res + one;
			res += one << 1;
		}
		res >>= 1;
		one >>= 2;
	}
	return res;
}

unsigned long long digsumodd(int n) {
	typedef unsigned long long u64;
	static u64 predigsum[10] = { 0, 25, 475, 7000, 92500, 1150000, 13750000, 160000000, 1825000000, 20500000000 };
	static u64 predigcnt[10] = { 0, 5, 50, 500, 5000, 50000, 500000, 5000000, 50000000, 500000000 };
	static u64 presumnum[10] = { 0, 1, 3, 6, 10, 15, 21, 28, 36, 45 };
	static u64 presumodd[10] = { 0, 1, 1, 4, 4, 9, 9, 16, 16, 25 };

	int scale = 0;
	for (int x = n; x; x /= 10)scale++;

	u64 sum = 0;
	while (--scale) {
		int d = (n / predigcnt[scale]) / 2;
		if (d)
			sum += predigsum[scale] * d + presumnum[d - 1] * predigcnt[scale];
		n -= predigcnt[scale] * 2 * d;
		if (d)
			sum += ((n + 1) / 2)*d;
	}
	sum += presumodd[n];
	return sum;
}

#ifdef _MSC_VER
#include <intrin.h>
#include <cstdint>
uint32_t __inline __builtin_clz(uint32_t value) {
	unsigned long leading_zero = 0;

	if (_BitScanReverse(&leading_zero, value))
		return 31 - leading_zero;

	// Undefined, 32 seems better than 0
	return 32;
}
#endif

#include <tuple>

// undefined for 0
auto encodeEliasDelta32(uint32_t x) {
	uint32_t n = 31 - __builtin_clz(x);
	x = ((n + 1) << n) | (x - (1 << n));
	n = n + 1 + ((31 - __builtin_clz(n + 1)) << 1);
	return std::make_pair(x, n);
}

uint32_t decodeEliasDelta32(uint32_t x) {
	uint32_t l = __builtin_clz(x);
	uint32_t n = (x >> (32 - 2 * l - 1)) - 1;
	return (1 << n) | ((x >> (32 - 2 * l - 1 - n)) & ((1 << n) - 1));
}