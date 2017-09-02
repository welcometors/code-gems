// https://projecteuler.net/problem=77
/*
It is possible to write ten as the sum of primes in exactly five different ways:

7 + 3
5 + 5
5 + 3 + 2
3 + 3 + 2 + 2
2 + 2 + 2 + 2 + 2

What is the first value which can be written as the sum of primes in over five 
thousand different ways?

Solution:
Similar to coin change problem.

*/

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using natural = unsigned;

const vector<natural> primes = {
	  2,      3,      5,      7,     11,     13,     17,     19,     23,     29,
 	 31,     37,     41,     43,     47,     53,     59,     61,     67,     71
};

auto compute() {
	constexpr size_t lim = 71;
	const size_t p = primes.size();
	vector<vector<natural>> table(lim + 1, vector<natural>(p, 0));
	for (size_t i = primes.front(); i <= lim; ++i) {
		natural ways = 0;
		for (size_t j = 0; j < p; ++j) {
			if (i == primes[j])
				ways++;
			else if (i > primes[j])
				ways += table[i - primes[j]][j];
			table[i][j] = ways;
		}
	}
	return table.back().back();
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6
		<< " miliseconds." << endl;
	cout << result << endl;
}