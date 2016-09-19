// https://projecteuler.net/problem=71
/*
Ordered fractions

Consider the fraction, n/d, where n and d are positive integers. If n<d and HCF(n,d)=1,
it is called a reduced proper fraction.

If we list the set of reduced proper fractions for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that 2/5 is the fraction immediately to the left of 3/7.

By listing the set of reduced proper fractions for d <= 1,000,000 in ascending order of size,
find the numerator of the fraction immediately to the left of 3/7.

Solution:
*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long natural;

constexpr natural limit = 1000000;

pair<natural, natural> prev2(pair<natural, natural> fraction) {
	natural maxNum = 0, maxDen = 1;
	for (natural d = limit, md = 1; d >= md; d--) {
		natural n = (d*fraction.first - 1) / fraction.second;
		if (n*maxDen > d*maxNum)
			maxNum = n, maxDen = d, md = d / (fraction.first*d - fraction.second*n) + 1;
	}
	return{ maxNum, maxDen };
}

natural compute() {
	natural num = 3, den = 7;
	natural maxNum = 0, maxDen = 1;

	for (natural d = 2; d <= limit; d++) {
		natural x = (d*num - 1) / den;
		if (x*maxDen > d*maxNum)
			maxNum = x, maxDen = d;
	}

	return maxNum;
}

int main() {
	auto begin = chrono::high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - begin).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}