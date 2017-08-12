// https://projecteuler.net/problem=43
/*
Sub-string divisibility

The number, 1406357289, is a 0 to 9 pandigital number because it is made up of
each of the digits 0 to 9 in some order, but it also has a rather interesting
sub-string divisibility property.

Let d1 be the 1st digit, d2 be the 2nd digit, and so on. In this way,
we note the following:

d2d3d4=406 is divisible by 2
d3d4d5=063 is divisible by 3
d4d5d6=635 is divisible by 5
d5d6d7=357 is divisible by 7
d6d7d8=572 is divisible by 11
d7d8d9=728 is divisible by 13
d8d9d10=289 is divisible by 17

Find the sum of all 0 to 9 pandigital numbers with this property.

Solution:
Start with the last 3 digits as 017, 034, 051, ..., 986
Add 1 digit at a time in the front and check for divisibility.
This will yield in 122 calls as opposed to 10! = 3628800 calls.
*/

#include <iostream>
#include <chrono>
using namespace std;

constexpr unsigned DIGITS = 9;
const unsigned divisor[] = { 1,2,3,5,7,11,13,17 };

uint64_t checkAndGet(unsigned index, unsigned number, unsigned last2, unsigned power, unsigned digitMask) {
	if (index == 0) {
		uint64_t fullNumber = power * 10;
		for (unsigned i = 0; i <= DIGITS; ++i) {
			if (!(digitMask & (1 << i))) {
				fullNumber *= i;
				break;
			}
		}
		fullNumber += number;
		return fullNumber;
	}

	power *= 10;
	uint64_t sum = 0;
	for (unsigned i = 0; i <= DIGITS; ++i, last2 += 100)
		if (!(digitMask & (1 << i)) && last2 % divisor[index] == 0)
			sum += checkAndGet(index - 1, number + i*power, last2 / 10, power, digitMask | (1 << i));
	return sum;
}

uint64_t compute() {
	uint64_t sum = 0;
	for (unsigned last3 = divisor[DIGITS - 2]; last3 < 1000; last3 += divisor[DIGITS - 2]) {
		const unsigned d1 = last3 % 10;
		if (d1 <= DIGITS) {
			const unsigned d2 = (last3 / 10) % 10;
			if (d2 <= DIGITS && d1 != d2) {
				const unsigned d3 = (last3 / 100) % 10;
				if (d3 <= DIGITS && d3 != d2 && d3 != d1)
					sum += checkAndGet(DIGITS - 3, last3, last3 / 10, 100, (1 << d1) | (1 << d2) | (1 << d3));
			}
		}
	}
	return sum;
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
