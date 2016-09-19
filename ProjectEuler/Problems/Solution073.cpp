// https://projecteuler.net/problem=73
/*
Ordered fractions

Consider the fraction, n/d, where n and d are positive integers. If n<d and HCF(n,d)=1, it is called a reduced proper fraction.

If we list the set of reduced proper fractions for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that there are 3 fractions between 1/3 and 1/2.

How many fractions lie between 1/3 and 1/2 in the sorted set of reduced proper fractions for d <= 12,000?

Solution:
*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long natural;
typedef unsigned long long ull;

constexpr natural limit = 12000;

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

ull indexDiff(pair<unsigned, unsigned> str, pair<unsigned, unsigned> end) {
	ull sum = 0;
	for (unsigned d = 2; d <= limit; d++) {
		unsigned s = (d*str.first + str.second - 1) / str.second;
		unsigned e = (d*end.first - 1) / end.second;
		for (unsigned i = s; i <= e; i++)
			if (GCD(i, d) == 1)
				sum++;
	}
	return sum - 1;
}

natural compute() {
	return indexDiff({ 1,3 }, { 1,2 });
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}