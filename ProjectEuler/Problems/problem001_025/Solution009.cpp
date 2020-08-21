// https://projecteuler.net/problem=9
/*
A Pythagorean triplet is a set of three natural numbers, a < b < c, for which,

a^2 + b^2 = c^2
For example, 3^2 + 4^2 = 9 + 16 = 25 = 5^2.

There exists exactly one Pythagorean triplet for which a + b + c = 1000.
Find the product abc.

Solution:

*/

#include <iostream>
#include <cmath>

using namespace std;
using natural = unsigned;

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

natural getMaxTriplet(natural s) {
	natural max = 0;
	natural s2 = s / 2;
	natural lim = ceil(sqrt(s*.5)) - 1;
	for (natural m = 2; m <= lim; m++) {
		if (s2%m == 0) {
			natural sm = s2 / m;
			while (sm % 2 == 0)
				sm /= 2;
			natural k = m + 1 + (m & 1);
			while (k < 2 * m && k <= sm) {
				if (sm%k == 0 && GCD(k, m) == 1) {
					natural d = s2 / (k*m);
					natural n = k - m;
					natural abc = d*(m*m - n*n) * 2 * d*m*n * d*(m*m + n*n);
					if (abc > max)
						max = abc;
				}
				k += 2;
			}
		}
	}
	return max;
}

int main() {
	cout << getMaxTriplet(1000) << '\n';
}