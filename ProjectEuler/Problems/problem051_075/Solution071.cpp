// https://projecteuler.net/problem=71
/*
Ordered fractions

Consider the fraction, n/d, where n and d are positive integers. If n<d and HCF(n,d)=1,
it is called a reduced proper fraction.

If we list the set of reduced proper fractions for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that 2/5 is the fraction immediately to the left of 3/7.

By listing the set of reduced proper fractions for d <= 1,000,000 in ascending order of size,
find the numerator of the fraction immediately to the left of 3/7.

Solution:
http://math.stackexchange.com/questions/39582/how-to-compute-next-previous-representable-rational-number

*/
#include <iostream>
#include <cstdint>
#include <chrono>

using namespace std;
typedef long long natural;

struct fraction {
	natural numerator;
	natural denominator;
};

template<class T>
T modInverse(T a, T m) {
	if (m == 1)
		return 0;

	T m0 = m;
	T x0 = 0, x1 = 1;
	while (a > 1) {
		T q = a / m;
		T t = m;
		m = a % m;
		a = t;

		t = x0;
		x0 = x1 - q * x0;
		x1 = t;
	}

	if (x1 < 0)
		x1 += m0;
	return x1;
}

auto previous(const fraction& f, natural n) {
	natural pinv = modInverse(f.numerator, f.denominator);
	natural mod = n % f.denominator;
	natural b = n - mod + pinv;
	if (mod < pinv)
		b -= f.denominator;
	natural a = (b * f.numerator - 1) / f.denominator;
	return fraction{ a, b };
}

auto compute() {
	const auto result = previous({ 3,7 }, 1'000'000ULL);
	return result.numerator;
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