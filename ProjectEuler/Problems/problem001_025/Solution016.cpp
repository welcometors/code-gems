// https://projecteuler.net/problem=16
/*
Power digit sum

2^15 = 32768 and the sum of its digits is 3 + 2 + 7 + 6 + 8 = 26.

What is the sum of the digits of the number 2^1000?

Solution:
*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<int> digitSumOf2Powers(int n) {
	const int numberOfMaxdigits = ceil(n * 0.30103);
	vector<int> digits(numberOfMaxdigits, 0);
	digits[0] = 1;
	vector<int> sumVector = { 1 };
	for (int i = 0, l = 1; i < n; i++) {
		int sum = 0, carry = 0;
		for (int j = 0; j < l; j++) {
			int val = digits[j] * 2 + carry;
			carry = val / 10;
			val %= 10;
			digits[j] = val;
			sum += val;
		}
		if (carry)
			digits[l++] = carry;
		sumVector.push_back(sum + carry);
	}
	return sumVector;
}

int main() {
	auto table = digitSumOf2Powers(1000);
	cout << table.back() << endl;
}