// https://projecteuler.net/problem=57
/*
Square root convergents

It is possible to show that the square root of two can be expressed as
an infinite continued fraction.

2^1/2 = 1 + 1/(2 + 1/(2 + 1/(2 + ... ))) = 1.414213...

By expanding this for the first four iterations, we get:

1 + 1/2 = 3/2 = 1.5
1 + 1/(2 + 1/2) = 7/5 = 1.4
1 + 1/(2 + 1/(2 + 1/2)) = 17/12 = 1.41666...
1 + 1/(2 + 1/(2 + 1/(2 + 1/2))) = 41/29 = 1.41379...

The next three expansions are 99/70, 239/169, and 577/408, but the eighth expansion,
1393/985, is the first example where the number of digits in the numerator exceeds
the number of digits in the denominator.

In the first one-thousand expansions, how many fractions contain a numerator
with more digits than denominator?

Solution:
*/

#include <iostream>
#include <cstdint>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <chrono>

auto compute() {
	using namespace boost::multiprecision;
	constexpr uint32_t limit = 1000;

	cpp_int num = 3, den = 2;
	uint32_t count = 0;
	for (uint32_t t = 2; t <= limit; ++t) {
		num += den << 1;
		den = num - den;
		if (num.str().length() > den.str().length()) {
			++count;
			//std::cout << t << ',';
		}
	}

	return count;
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