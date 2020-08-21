using namespace std;

#define natural unsigned int

#pragma region "BINARY GCD"

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

#pragma endregion

#pragma region "INTRINSICS GCD"

#include "Intrinsics.cpp"	// __builtin_ctz

unsigned int fastGCD(unsigned int u, unsigned int v){
	if (u == 0) return v;
	if (v == 0) return u;
	
	int shift = __builtin_ctz(u | v);
	u >>= __builtin_ctz(u);
	do {
		v >>= __builtin_ctz(v);
		if (u > v) {
			unsigned int t = v;
			v = u;
			u = t;
		}
		v = v - u;
	} while (v != 0);
	
	return u << shift;
}

#pragma endregion

#pragma region "FACTORS"

template<class T>
T getFactors(T n) {
	T factors = 1;
	for (auto prime : primes) {
		T power = 0;
		while (n % prime == 0) {
			n /= prime;
			power++;
		}
		if (power)
			factors *= power + 1;
		if (n == 1)
			break;
	}
	if (n > 1)
		factors *= 2;
	return factors;
}
#pragma endregion