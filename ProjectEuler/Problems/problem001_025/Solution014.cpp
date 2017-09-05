// https://projecteuler.net/problem=14
/*
Longest Collatz sequence

The following iterative sequence is defined for the set of positive integers:

n -> n/2 (n is even)
n -> 3n + 1 (n is odd)

Using the rule above and starting with 13, we generate the following sequence:

13 -> 40 -> 20 -> 10 -> 5 -> 16 -> 8 -> 4 -> 2 -> 1
It can be seen that this sequence (starting at 13 and finishing at 1) contains 10 terms.
Although it has not been proved yet (Collatz Problem), it is thought that all starting
numbers finish at 1.

Which starting number, under one million, produces the longest chain?

NOTE: Once the chain starts the terms are allowed to go above one million.

Solution:

*/

#include <iostream>
#include <vector>
using namespace std;

using natural = unsigned long long;
constexpr natural limit = 1000000;
constexpr natural tabelSize = 10000;
natural table[tabelSize] = {};

natural collatz(natural n) {
	if (n < tabelSize) {
		if (table[n])
			return table[n];
		if (n % 2 == 0)
			table[n] = collatz(n / 2) + 1;
		else
			table[n] = collatz(3 * n + 1) + 1;
	}
	else {
		if (n % 2 == 0)
			return collatz(n / 2) + 1;
		else
			return collatz(3 * n + 1) + 1;
	}
}

int prepareTable() {
	table[1] = 1;
	natural max = 0, num = 0;
	for (int i = 2; i < limit; i++) {
		natural len = collatz(i);
		if (len > max) {
			max = len;
			num = i;
		}
	}
	return num;
}

int main() {
	cout << prepareTable() << '\n';
}
