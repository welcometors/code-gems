// https://projecteuler.net/problem=66
/*
Diophantine equation

Solution:
*/

#include <iostream>
#include <cmath>
#include <chrono>
#include <boost\multiprecision\cpp_int.hpp>
using namespace boost::multiprecision;
using namespace std;
typedef unsigned int natural;

cpp_int solve(natural n) {
	natural sqrtn = sqrt(n);
	if (sqrtn*sqrtn == n)
		return 0;

	natural m = 0, d = 1, a = sqrtn;
	cpp_int num_1 = 1, den_1 = 0;
	cpp_int num0 = a, den0 = 1;
	while (num0*num0 - den0*den0*n != 1) {
		m = d*a - m;
		d = (n - m*m) / d;
		a = (sqrtn + m) / d;

		cpp_int num_2 = num_1;
		num_1 = num0;
		num0 = a*num_1 + num_2;
		cpp_int den_2 = den_1;
		den_1 = den0;
		den0 = a*den_1 + den_2;
	}

	return num0;
}

int main() {
	auto begin = chrono::high_resolution_clock::now();

	cpp_int max = 0, val;
	natural maxd = 0;
	for (natural d = 2; d <= 1000; d++)
		if ((val = solve(d)) > max)
			max = val, maxd = d;
	cout << maxd << ": " << max << endl;

	cout << "Done in " << chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - begin).count() / 1000000.0 << " miliseconds." << endl;
}