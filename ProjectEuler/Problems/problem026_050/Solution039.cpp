// https://projecteuler.net/problem=39
/*
Integer right triangles

If p is the perimeter of a right angle triangle with integral length sides, {a,b,c}, 
there are exactly three solutions for p = 120.

{20,48,52}, {24,45,51}, {30,40,50}

For which value of p <= 1000, is the number of solutions maximised?

Solution:
Generate all the Pythagorean triples using primitive Pythagorean triples.
Create a map of a+b+c to count number of appearance.
Get the max.
*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

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

typedef unsigned natural;
natural compute() {
	const natural s = 1000;
	unordered_map<natural, natural> map;
	natural s2 = s / 2;
	natural lim = ceil(sqrt(s*.5)) - 1;
	for (natural m = 2; m <= lim; m++) {
		for (natural n = 1; n < m; n++) {
			if (GCD(m, n) == 1) {
				natural a = m*m - n*n, b = 2 * m*n, c = m*m + n*n;
				if (GCD(a, b) == 1) {
					if (s2 < c)
						break;
					for (natural r = 1, k = a + b + c; r <= s / c && r*k <= s; r++) {
						map[r*k]++;
						//printf("%4d: %d^2 x %d^2 = %d^2\n", r*(a + b + c), r*a, r*b, r*c);
					}
				}
			}
		}
	}

	vector<pair<natural, natural>> values;
	for (auto p : map)
		values.push_back(p);
	map.clear();

	sort(values.begin(), values.end(), [](const auto& a, const auto&b) {
		return a.first < b.first;
	});

	vector<natural> maxTable = { 0 };
	natural max = 0;
	for (auto v : values) {
		if (v.second > max) {
			max = v.second;
			maxTable.push_back(v.first);
			//printf("%5d, ", v.first);
		}
	}
	return maxTable.back();
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