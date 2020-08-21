// https://projecteuler.net/problem=48
/*
Self powers

The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.

Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.

Solution:

*/

#include <iostream>
#include <chrono>
using namespace std;

using natural = unsigned long long;
constexpr natural Modulus = 10000000000ULL;
constexpr size_t ModLimitBit = 30;
constexpr natural ModLimit = 1 << ModLimitBit;

inline natural modAdd(const natural x, const natural y) {
	natural z = x + y;
	if (z >= Modulus)
		z -= Modulus;
	return z;
}

inline natural modMul(const natural x, const natural y) {
	if (x > ModLimit && y > ModLimit) // handles overflow
		return ((x >> ModLimitBit)*((y << ModLimitBit) % Modulus) + y*(x & (ModLimit - 1))) % Modulus;
	natural z = x*y;
	if (z >= Modulus)
		z %= Modulus;
	return z;
}

natural modPow(natural i, natural j) {
	if (!j)
		return 1;
	if (i < 2)
		return i;
	natural p = 1;
	while (j) {
		if (j & 1)
			p = modMul(p, i);
		i = modMul(i, i);
		j >>= 1;
	}
	return p;
}

natural compute() {
	natural sum = 0;
	for (natural x = 1; x <= 1000; ++x)
		sum = modAdd(sum, modPow(x, x));
	return sum;
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

