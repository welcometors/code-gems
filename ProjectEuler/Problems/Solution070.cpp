// https://projecteuler.net/problem=70
/*
Totient permutation

Euler's Totient function, phi(n) [sometimes called the phi function], is used to determine the number
of positive numbers less than or equal to n which are relatively prime to n. For example, as 1, 2, 4, 5,
7, and 8, are all less than nine and relatively prime to nine, phi(9)=6.

The number 1 is considered to be relatively prime to every positive number, so phi(1)=1.

Interestingly, phi(87109)=79180, and it can be seen that 87109 is a permutation of 79180.

Find the value of n, 1 < n < 10^7, for which phi(n) is a permutation of n and the ratio n/phi(n) produces a minimum.

Solution:
For the ratio n/phi(n) to be the smallest n should be prime. Because, phi(prime)=prime-1. But n can't be prime
due to n can't be a permutation of n-1. So, phi(n) has to be dividend among primes. So, to phi(n) be largest it
should contain as less primes as possible, i.e. two.
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>
#include <chrono>
using namespace std;
typedef unsigned int natural;
constexpr natural limit = 10000000;
constexpr natural sqrtLimit = 3163;
constexpr natural searchLimit = 9 * sqrtLimit;
constexpr natural sqrtSearchLimit = 3 * sqrtLimit;

vector<int> primes;
bitset<searchLimit + 1> isPrime;
void populatePrimes() {
	isPrime.set();
	isPrime.reset(0);
	isPrime.reset(1);
	primes.clear();

	for (int i = 2; i <= sqrtSearchLimit; i++) {
		if (!isPrime.test(i))
			continue;
		primes.push_back(i);
		for (int j = i + i; j <= searchLimit; j += i)
			isPrime.reset(j);
	}

	for (int i = sqrtSearchLimit + 1; i <= searchLimit; i++) {
		if (isPrime.test(i))
			primes.push_back(i);
	}
}

bool isPermutation(natural a, natural b) {
	int dc[10] = {};
	while (a) {
		dc[a % 10]++;
		a /= 10;
	}
	while (b) {
		dc[b % 10]--;
		b /= 10;
	}
	for (auto v : dc)
		if (v)
			return false;
	return true;
}

int main() {
	auto begin = chrono::high_resolution_clock::now();

	populatePrimes();
	double maxInverseRatio = 0;
	natural maxn = 0;
	for (natural i = 1, size = primes.size(); i < size; i++)
		for (natural j = 0, n; j < i && (n = primes[i] * primes[j]) < limit; j++) {
			natural t = n - primes[j] - primes[i] + 1;
			if (isPermutation(n, t) && (double)t / n > maxInverseRatio)
				maxn = n, maxInverseRatio = (double)t / n;
		}

	cout << "Done in " << chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - begin).count() / 1000000.0 << " miliseconds." << endl;
	cout << maxn << endl;
}