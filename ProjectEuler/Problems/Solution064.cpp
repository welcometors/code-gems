// https://projecteuler.net/problem=64
/*
Odd period square roots

All square roots are periodic when written as continued fractions and can be written in the form:
<visit html page>

It can be seen that the sequence is repeating. For conciseness, we use the notation sqrt(23) = [4;(1,3,1,8)],
to indicate that the block (1,3,1,8) repeats indefinitely.

The first ten continued fraction representations of (irrational) square roots are:
Number		Form			Period
--------------------------------------
sqrt( 2)	[1;(2)] 		1
sqrt( 3)	[1;(1,2)] 		2
sqrt( 5)	[2;(4)] 		1
sqrt( 6)	[2;(2,4)] 		2
sqrt( 7)	[2;(1,1,1,4)] 	4
sqrt( 8)	[2;(1,4)] 		2
sqrt(10)	[3;(6)] 		1
sqrt(11)	[3;(3,6)] 		2
sqrt(12)	[3;(2,6)] 		2
sqrt(13)	[3;(1,1,1,1,6)] 5

Exactly four continued fractions, for N <= 13, have an odd period.

How many continued fractions for N <= 10000 have an odd period?

Solution:
*/

#include <iostream>
#include <cstdint>
#include <cmath>
#include <unordered_set>
#include <chrono>
using namespace std;

struct EdgeHash {
	size_t operator() (const pair<int, int> &data) const { // Cantor pairing
		return (data.first + data.second)*(data.first + data.second + 1) / 2 + data.second;
	}
};

int getChainLength(int n) {
	int sqrtn = sqrt(n);
	if (sqrtn*sqrtn == n)
		return 0;

	int a = sqrtn, b = 1; // form is b/[sqrt(n)-a]
	unordered_set<pair<int, int>, EdgeHash> mem = { { a,b } };
	for (int i = 1; ; i++) {
		b = (n - a*a) / b;
		a = ((sqrtn + a) / b)*b - a;
		if (mem.find({ a,b }) != mem.end())
			return i;
		mem.insert({ a,b });
	}
}

auto compute() {
	unsigned c = 0;
	for (int i = 2; i <= 30000; i++)
		if (getChainLength(i) & 1)
			c++;
	return c;
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