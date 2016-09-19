// https://projecteuler.net/problem=34
/*
Digit factorials

145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.

Find the sum of all numbers which are equal to the sum of the factorial of their digits.

Note: as 1! = 1 and 2! = 2 are not sums they are not included.

Solution:

*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

typedef unsigned long natural;
const unsigned fact[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880 };

int checkAndGet(int n, int s, int d) {
	if (!d--) {
		if (n == s)
			cout << n << endl;
		return n == s ? n : 0;
	}

	n *= 10;
	int sum = 0;
	for (int i = n ? 0 : 1; i < 10; i++)
		sum += checkAndGet(n + i, s + fact[i], d);
	return sum;
}

int compute() {
	int sum = 0;
	for (int i = 2; i <= 5; i++)
		sum += checkAndGet(0, 0, i);
	return sum;
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