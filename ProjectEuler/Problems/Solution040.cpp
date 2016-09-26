// https://projecteuler.net/problem=40
/*
Champernowne's constant

An irrational decimal fraction is created by concatenating the positive integers:

0.123456789101112131415161718192021...

It can be seen that the 12th digit of the fractional part is 1.

If dn represents the nth digit of the fractional part, find the value of the following expression.

d1 × d10 × d100 × d1000 × d10000 × d100000 × d1000000

Solution:
*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

typedef unsigned natural;
unsigned d(natural n) {
	unsigned d = 1;
	natural size = 9, start = 1;
	while (n >= size*d) {
		start += size;
		n -= d*size;
		size *= 10;
		d++;
	}
	start += n / d;
	d = d - 1 - n%d;
	while (d--)
		start /= 10;
	return start % 10;
}

natural compute() {
	natural product = 1;
	for (natural i = 1, p = 1; p <= 7; p++, i *= 10)
		product *= d(i - 1);
	return product;
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	auto end = high_resolution_clock::now();
	cout << result << '\n';
	cout << "Done in "
		<< duration_cast<nanoseconds>(end - start).count() / 1000000.0
		<< " miliseconds." << '\n';
}