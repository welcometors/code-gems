// https://projecteuler.net/problem=45
/*
Triangular, pentagonal, and hexagonal

Triangle, pentagonal, and hexagonal numbers are generated by the following formulae:

Triangle	 	Tn=n(n+1)/2	 	1, 3,  6, 10, 15, ...
Pentagonal	 	Pn=n(3n-1)/2	1, 5, 12, 22, 35, ...
Hexagonal	 	Hn=n(2n-1)	 	1, 6, 15, 28, 45, ...

It can be verified that T285 = P165 = H143 = 40755.

Find the next triangle number that is also pentagonal and hexagonal.

Solution:

*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;

template <class T>
inline T hexagonal(const T x) {
	return x*(2 * x - 1);
}

template <class T>
T hexagonalIndex(T x) {
	x = 8 * x + 1;
	const T r = sqrt(x);
	if (r*r != x)
		return 0;
	return (r + 1) % 4 == 0 ? (r + 1) / 4 : 0;
}

template <class T>
T pentagonalIndex(T x) {
	x = 24 * x + 1;
	const T r = sqrt(x);
	if (r*r != x)
		return 0;
	return (r + 1) % 6 == 0 ? (r + 1) / 6 : 0;
}

template <class T>
T triangleIndex(T x) {
	x = 1 + 8 * x;
	const T r = sqrt(x);
	return (r*r == x && r % 2 == 1) ? (r - 1) / 2 : 0;
}

using natural = unsigned long long;
natural compute() {
	constexpr natural start = 144;
	for (natural h = hexagonal(start), d = hexagonal(start + 1) - h; ; h += d, d += 4) {
		if (pentagonalIndex(h) && triangleIndex(h)) {
			//cout << 'T' << triangleIndex(h) << " = P" << pentagonalIndex(h) << " = H" << hexagonalIndex(h) << '\n';
			return h;
		}
	}
	return 0;
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

