// https://projecteuler.net/problem=52
/*
Permuted multiples

It can be seen that the number, 125874, and its double, 251748, 
contain exactly the same digits, but in a different order.

Find the smallest positive integer, x, such that 2x, 3x, 4x, 5x, 
and 6x, contain the same digits.

Solution:

*/

#include <iostream>
#include <cstdint>
#include <cmath>
#include <chrono>
using namespace std;

using natural = uint32_t;

template<class T>
bool isPermutation(T x, T y) {
	unsigned char count[10] = {};
	while (x && y) {
		count[x % 10]++;
		count[y % 10]--;
		x /= 10;
		y /= 10;
	}
	if (x || y)
		return false;
	for (const auto& c : count)
		if (c)
			return false;
	return true;
}

auto find(natural n, natural k) {
	const natural limit = pow(10, n);
	for (natural x = limit / 10 + 1; x < limit / k; ++x) {
		natural m = 2;
		for (; m <= k; ++m)
			if (!isPermutation(x, x*m))
				break;
		if (m > k)
			return x;
	}
}

auto compute() {
	return find(6, 6);
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
