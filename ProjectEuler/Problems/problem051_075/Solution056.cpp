// https://projecteuler.net/problem=56
/*
Powerful digit sum

A googol (10^100) is a massive number:
one followed by one-hundred zeros;

100^100 is almost unimaginably large:
one followed by two-hundred zeros.

Despite their size, the sum of the digits in each number is only 1.

Considering natural numbers of the form, a^b, where a, b < 100,
what is the maximum digital sum?

Solution:

*/

#include <iostream>
#include <cstdint>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <chrono>

auto digitCount(uint32_t a, uint32_t b) {
	using namespace boost;
	multiprecision::cpp_int p = a;
	p = multiprecision::pow(p, b);
	auto ans = p.str();
	uint32_t sum = 0;
	for (const auto &c : ans)
		sum += c - '0';
	return sum;
}

auto compute() {
	constexpr uint32_t limit = 100;

	uint32_t max = 0;
	for (uint32_t a = 1; a < limit; ++a)
		for (uint32_t b = 1, c; b < limit; ++b)
			if (max < (c = digitCount(a, b)))
				max = c;

	return max;
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