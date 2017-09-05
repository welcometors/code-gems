// https://projecteuler.net/problem=30
/*
Digit fifth powers

Surprisingly there are only three numbers that can be written as the sum of fourth powers of their digits:

1634 = 1^4 + 6^4 + 3^4 + 4^4
8208 = 8^4 + 2^4 + 0^4 + 8^4
9474 = 9^4 + 4^4 + 7^4 + 4^4
As 1 = 1^4 is not a sum it is not included.

The sum of these numbers is 1634 + 8208 + 9474 = 19316.

Find the sum of all the numbers that can be written as the sum of fifth powers of their digits.

Solution:
Since 6x9^5 <= 10^6, All those numbers should be less than 10^6.

*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long natural;
typedef unsigned long long ull;
typedef long long ll;

constexpr int power(int x) {
	return x*x*x*x*x;
}

const int powerTable[] = {
	power(0), power(1), power(2), power(3), power(4),
	power(5), power(6), power(7), power(8), power(9)
};

int checkAndGet(int n, int s, int d) {
	if (!d)
		return n == s ? n : 0;

	d--;
	n *= 10;
	int sum = 0;
	for (int i = 0; i < 10; i++, n++)
		sum += checkAndGet(n, s + powerTable[i], d);
	return sum;
}

auto compute() {
	return checkAndGet(0, 0, 6) - 1;
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
