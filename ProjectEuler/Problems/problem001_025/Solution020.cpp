// https://projecteuler.net/problem=20
/*
Factorial digit sum

n! means n x (n - 1) x ... x 3 x 2 x 1

For example, 10! = 10 x 9 x ... x 3 x 2 x 1 = 3628800,
and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.
Find the sum of the digits in the number 100!

Solution:
*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

auto digitSumOfFactorial(int n) {
	const int numberOfMaxdigits = ceil(n * log10(n));
	vector<int> digits(numberOfMaxdigits, 0);
	digits[0] = 1;
	vector<int> sumVector = { 1,1 };
	for (int i = 2, l = 1; i <= n; i++) {
		int sum = 0, carry = 0;
		for (int j = 0; j < l; j++) {
			carry += digits[j] * i;
			sum += (digits[j] = carry % 10);
			carry /= 10;
		}
		for (; carry; carry /= 10)
			sum += (digits[l++] = carry % 10);
		sumVector.push_back(sum);
	}
	return sumVector;
}

int main() {
	auto table = digitSumOfFactorial(100);
	cout << table.back() << '\n';
}