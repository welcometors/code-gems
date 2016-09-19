// https://projecteuler.net/problem=38
/*
Pandigital multiples

Take the number 192 and multiply it by each of 1, 2, and 3:

192 × 1 = 192
192 × 2 = 384
192 × 3 = 576
By concatenating each product we get the 1 to 9 pandigital, 192384576. 
We will call 192384576 the concatenated product of 192 and (1,2,3)

The same can be achieved by starting with 9 and multiplying by 1, 2, 3, 4, and 5, 
giving the pandigital, 918273645, which is the concatenated product of 9 and (1,2,3,4,5).

What is the largest 1 to 9 pandigital 9-digit number that can be formed as the 
concatenated product of an integer with (1,2, ... , n) where n > 1?

Solution:

*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

template<class T>
T concat(T a, T b) {
	T c = b;
	while (c) {
		a *= 10;
		c /= 10;
	}
	return a + b;
}

// will return <status,concatenated-number>
// status = -1 if number of digits in concatenated-number exceeds 9
// status =  1 else if concatenated-number is pandigital
// status =  0 otherwise
pair<int, unsigned> check(unsigned n, unsigned k) {
	unsigned digits = 0, numOfDigits = 0, number = 0, power = 1;
	for (unsigned i = 1; i <= k; i++) {
		unsigned product = n*i;
		number = concat(number, product);
		while (product) {
			unsigned mask = 1 << (product % 10);
			digits ^= mask;
			numOfDigits++;
			power *= 10;
			product /= 10;
		}
	}
	return{ numOfDigits > 9 ? -1 : (digits == 0x3FE), number };
}

unsigned compute() {
	unsigned max = 0, k = 5;
	for (unsigned i = 1; ; i++) {
		auto result = check(i, k);
		if (result.first < 0) {
			k--;
			if (k < 2)
				break;
		}
		else if (result.first) {
			//cout << i << " x 1..." << k << " = " << result.second << endl;
			if (result.second > max)
				max = result.second;
		}
	}
	return max;
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