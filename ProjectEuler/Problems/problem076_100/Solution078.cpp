// https://projecteuler.net/problem=78
/*
Let p(n) represent the number of different ways in which n coins can be 
separated into piles. For example, five coins can be separated into piles 
in exactly seven different ways, so p(5) = 7.

OOOOO
OOOO O
OOO	OO
OOO O O
OO OO O
OO O O O
O O O O O

Find the least value of n for which p(n) is divisible by one million.

Solution:
The sequence is "Partition Number" sequence.
p(n) = p(n-1) + p(n-2) - p(n-5) - p(n-7) + p(n-12) + p(n-15) - p(n-22) ...
Numbers are of the form m(3m - 1)/2, where m is an integer.
The signs in the summation alternate as (-1)^(|m|-1)

*/

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

using natural = unsigned long long;
constexpr natural mod = 1'000'000;
constexpr size_t lim = 100'000;

natural compute() {
	vector<natural> p = { 1 };
	p.reserve(lim);

	for (size_t i = 1; i <= lim; ++i) {
		// taking 2 pantagonal numbers as t1 and t2
		size_t t1 = 1, t2 = 2, d1 = 4, d2 = 5;
		natural s = 0;
		do {
			s += p[i - t1];
			t1 += d1;
			d1 += 3;
			if (t2 > i)
				break;
			s += p[i - t2];
			t2 += d2;
			d2 += 3;
			// unrolling next 2 to avoid branch
			if (t1 > i)
				break;
			s += mod - p[i - t1];
			t1 += d1;
			d1 += 3;
			if (t2 > i)
				break;
			s += mod - p[i - t2];
			t2 += d2;
			d2 += 3;
		} while (t1 <= i);
		s %= mod;
		if (!s)
			return i;
		p.push_back(s);
	}

	return 0;
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

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