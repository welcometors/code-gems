// https://projecteuler.net/problem=26
/*
Reciprocal cycles

A unit fraction contains 1 in the numerator. The decimal representation of the unit fractions with denominators 2 to 10 are given:

1/2	= 	0.5
1/3	= 	0.(3)
1/4	= 	0.25
1/5	= 	0.2
1/6	= 	0.1(6)
1/7	= 	0.(142857)
1/8	= 	0.125
1/9	= 	0.(1)
1/10	= 	0.1
Where 0.1(6) means 0.166666..., and has a 1-digit recurring cycle. It can be seen that 1/7 has a 6-digit recurring cycle.

Find the value of d < 1000 for which 1/d contains the longest recurring cycle in its decimal fraction part.

Solution:
*/

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <algorithm>
using namespace std;
typedef unsigned int natural;

vector<int> populatePrimes(int limit) {
	vector<int> primes;
	int sqrtLimit = sqrt(limit + 1) + 1;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset(&isPrime[2], 1, sizeof(int)*(limit - 1));
	primes.clear();

	for (int i = 2; i <= sqrtLimit; i++) {
		if (!isPrime[i])
			continue;

		primes.push_back(i);

		for (int j = i + i; j <= limit; j += i)
			isPrime[j] = false;
	}

	for (int i = sqrtLimit + 1; i <= limit; i++) {
		if (isPrime[i])
			primes.push_back(i);
	}

	return primes;
}

natural getRecurringLength(natural n) {
	natural dividend = 1;
	for (natural i = 0; i<n; i++) {
		dividend = (dividend * 10) % n;
		if (!dividend)
			return 0;
	}
	for (natural i = 1, initial = dividend; ; i++) {
		dividend = (dividend * 10) % n;
		if (dividend == initial)
			return i;
	}
	return -1;
}

vector<natural> getChainTable(natural limit) {
	auto primes = populatePrimes(limit);
	vector<natural> table = { 0 };
	natural max = 0;
	for (auto prime : primes) {
		natural l = getRecurringLength(prime);
		if (l > max) {
			table.push_back(prime);
			max = l;
		}
	}
	return table;
}

int main() {
	constexpr natural limit = 1000;
	auto table = getChainTable(limit);
	cout << *(lower_bound(table.begin(), table.end(), 1000) - 1) << endl;
}