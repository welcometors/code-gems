// https://projecteuler.net/problem=206
/*
Concealed Square
Find the unique positive integer whose square has the form 1_2_3_4_5_6_7_8_9_0,
where each “_” is a single digit.

Solution:
Since last digit is 0, so the last digit of square-root of it should also be 0.
Now, search through the remainings.
*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long natural;
typedef unsigned long long ull;
typedef long long ll;

int check(ll n) {
	n *= n;
	ll r = 0, c = 0;
	while (n) {
		r = r * 10 + n % 10;
		n /= 10;
		c++;
	}
	if (c != 17)
		return c < 17 ? -1 : 1;
	c = 1;
	while (r) {
		if (r % 10 != c)
			return r % 10 < c ? -1 : 1;
		r /= 100;
		c++;
	}
	return c != 10;
}
// 138902662
// 101010101
ll compute() {
	ll l = sqrt(10203040506070809LL), r = sqrt(19293949596979899LL);
	l = l / 10 + 3;
	while (l < r) {
		if (!check(l))
			break;
		l += 4;
		if (!check(l))
			break;
		l += 6;
	}
	return l;
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
	cout << result*result << endl;
}