// https://projecteuler.net/problem=66
/*
Diophantine equation

Consider quadratic Diophantine equations of the form:

x^2 – Dy^2 = 1

For example, when D=13, the minimal solution in x is 649^2 – 13×180^2 = 1.

It can be assumed that there are no solutions in positive integers when D is square.

By finding minimal solutions in x for D = {2, 3, 5, 6, 7}, we obtain the following:

32 – 2×22 = 1
22 – 3×12 = 1
92 – 5×42 = 1
52 – 6×22 = 1
82 – 7×32 = 1

Hence, by considering minimal solutions in x for D <= 7, the largest x is obtained when D=5.

Find the value of D <= 1000 in minimal solutions of x for which the largest value of x is obtained.

Solution:
Pell Equation
https://www.uni-oldenburg.de/fileadmin/user_upload/mathe/personen/steffen.mueller/01lenstra.pdf
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