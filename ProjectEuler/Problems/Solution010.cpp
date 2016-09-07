// https://projecteuler.net/problem=10
/*
The sum of the primes below 10 is 2 + 3 + 5 + 7 = 17.

Find the sum of all the primes below two million.

Solution:

*/

#include <vector>
#include <iostream>
#include <memory>
#include <cstring>
#include <cmath>
using namespace std;

typedef unsigned long long natural;
const natural limit = 2000000;

natural populatePrimes(int limit) {
	int sqrtLimit = sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);
	natural sum = 0;

	isPrime[0] = isPrime[1] = 0;
	std::memset(&isPrime[2], 1, sizeof(int)*(limit - 1));

	for (int i = 2; i <= sqrtLimit; i++) {
		if (!isPrime[i])
			continue;

		sum += i;

		for (int j = i + i; j <= limit; j += i)
			isPrime[j] = false;
	}

	for (int i = sqrtLimit + 1; i <= limit; i++) {
		if (isPrime[i])
			sum += i;
	}

	return sum;
}

int main() {
	cout << populatePrimes(limit) << endl;
}
