// https://projecteuler.net/problem=53
/*
Combinatoric selections

There are exactly ten ways of selecting three from five, 12345:

123, 124, 125, 134, 135, 145, 234, 235, 245, and 345

In combinatorics, we use the notation, 5C3 = 10.

In general,

nCr = n!/r!(n-r)!,
where r <= n, n! = nx(n-1)x...x3x2x1, and 0! = 1.

It is not until n = 23, that a value exceeds one-million: 23C10 = 1144066.

How many, not necessarily distinct,
values of  nCr, for 1 <= n <= 100, are greater than one-million?

Solution:

*/

#include <iostream>
#include <cstdint>
#include <vector>
#include <chrono>
using namespace std;

using natural = uint32_t;
auto getCount(natural maxn, natural k) {
	natural count = 0;
	vector<natural> ncr(maxn + 1, 1);
	for (natural n = 2; n <= maxn; ++n) {
		for (natural r = n - 1; r >= 1; --r) {
			ncr[r] += ncr[r - 1];
			if (ncr[r] > k) {
				count += r - (n - r) + 1;
				ncr[r] = k;
				break;
			}
		}
	}
	return count;
}

auto compute() {
	return getCount(100, 1'000'000);
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	auto end = high_resolution_clock::now();
	cout << result << '\n';
	cout << "Done in "
		<< duration_cast<nanoseconds>(end - start).count() / 1000000.0
		<< " miliseconds." << '\n';
}
