// https://projecteuler.net/problem=32
/*
Pandigital products

We shall say that an n-digit number is pandigital if it makes use of all the digits 1 to n exactly once; 
for example, the 5-digit number, 15234, is 1 through 5 pandigital.

The product 7254 is unusual, as the identity, 39 x 186 = 7254, containing multiplicand, multiplier, 
and product is 1 through 9 pandigital.

Find the sum of all products whose multiplicand/multiplier/product identity can be written as a 1 through 9 pandigital.

HINT: Some products can be obtained in more than one way so be sure to only include it once in your sum.

Solution:

*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <unordered_set>
#include <chrono>
using namespace std;
typedef unsigned long long ull;
typedef long long ll;

typedef unsigned long natural;

bool containsZero(int n) {
	while (n) {
		if (n % 10 == 0)
			return true;
		n /= 10;
	}
	return false;
}

bool check(unsigned i, unsigned j, unsigned k) {
	unsigned digits = 0;
	while (k) {
		unsigned mask = 1 << (k % 10);
		if (digits & mask)
			return false;
		digits ^= mask;
		k /= 10;
	}
	while (i) {
		unsigned mask = 1 << (i % 10);
		if (digits & mask)
			return false;
		digits ^= mask;
		i /= 10;
	}
	while (j) {
		unsigned mask = 1 << (j % 10);
		if (digits & mask)
			return false;
		digits ^= mask;
		j /= 10;
	}
	return digits == 0x3FE;
}

natural compute() {
	unordered_set<int> set;
	for (int i = 1; i < 100; i++) {
		if (containsZero(i))
			continue;
		for (int j = 100, p; j < 10000 && (p = i*j) < 100000; j++) {
			if (check(i, j, p))
				set.insert(p);
		}
	}
	natural sum = 0;
	for (auto val : set)
		sum += val;
	return sum;
}

int main() {
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}