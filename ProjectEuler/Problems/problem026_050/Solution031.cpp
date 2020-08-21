// https://projecteuler.net/problem=31
/*
Coin sums

In England the currency is made up of pound, £, and pence, p, and there are eight coins in general circulation:

1p, 2p, 5p, 10p, 20p, 50p, £1 (100p) and £2 (200p).
It is possible to make £2 in the following way:

1×£1 + 1×50p + 2×20p + 1×5p + 1×2p + 3×1p
How many different ways can £2 be made using any number of coins?

Solution:

*/

#include <iostream>
#include <cmath>
#include <chrono>
#include <memory>
#include <cstring>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

typedef unsigned long natural;
const natural coins[] = { 1, 2, 5, 10, 20, 50, 100, 200 };

natural ways(int n) {
	unique_ptr<natural[]> table(new natural[n + 1]);
	memset(&table[0], 0, sizeof(natural)*(n + 1));
	table[0] = 1;

	for (auto coin : coins)
		for (natural x = coin; x <= n; x++)
			table[x] += table[x - coin];
	
	return table[n];
}

auto compute() {
	return ways(200);
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