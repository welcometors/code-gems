// https://projecteuler.net/problem=
/*
Solution:
*/

#include <iostream>
#include <cstdint>
#include <chrono>

auto compute() {
	return 0;
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