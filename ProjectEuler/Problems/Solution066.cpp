// https://projecteuler.net/problem=65
/*
Convergents of e

The square root of 2 can be written as an infinite continued fraction.

2^1/2 = 1 + 1/(2 + 1/(2 + 1/(2 + ... ))) = 1.414213...
<visit page>

Hence the sequence of the first ten convergents for sqrt(2) are:
1, 3/2, 7/5, 17/12, 41/29, 99/70, 239/169, 577/408, 1393/985, 3363/2378, ...

What is most surprising is that the important mathematical constant,
e = [2; 1,2,1, 1,4,1, 1,6,1 , ... , 1,2k,1, ...].

The first ten terms in the sequence of convergents for e are:

2, 3, 8/3, 11/4, 19/7, 87/32, 106/39, 193/71, 1264/465, 1457/536, ...
The sum of digits in the numerator of the 10th convergent is 1+4+5+7=17.

Find the sum of digits in the numerator of the 100th convergent of the continued fraction for e.

Solution:
*/

#include <iostream>
#include <cstdint>
#include <boost/multiprecision/cpp_int.hpp>
#include <chrono>

using namespace boost::multiprecision;
using natural = unsigned int;

cpp_int solve(natural n) {
	natural sqrtn = sqrt(n);
	if (sqrtn*sqrtn == n)
		return 0;

	natural f = 0, b = 1, a = sqrtn;
	cpp_int num_1 = 1, den_1 = 0;
	cpp_int num0 = a, den0 = 1;
	while (num0*num0 - den0*den0*n != 1) {
		f = b*a - f;
		b = (n - f*f) / b;
		a = (sqrtn + f) / b;

		cpp_int num_2 = num_1;
		num_1 = num0;
		num0 = a*num_1 + num_2;
		cpp_int den_2 = den_1;
		den_1 = den0;
		den0 = a*den_1 + den_2;
	}

	return num0;
}

auto compute() {
	cpp_int max = 0, val;
	natural maxd = 0;
	for (natural d = 2; d <= 1000; ++d) {
		if ((val = solve(d)) > max) {
			max = val;
			maxd = d;
		}
	}
	//std ::cout << maxd << ": " << max << endl;
	return maxd;
}

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}