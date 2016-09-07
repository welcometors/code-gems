// https://projecteuler.net/problem=7
/*
By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, we can see that the 6th prime is 13.

What is the 10001st prime number?

Facts:
All primes greater than 3 can be written in the form 6.k+/-1.
Any number n can have only one primefactor greater than n.

Solution:
we can maintain a prime table so that we'll check wheather a number is prime or not is by only
checking against prime untill sqrt(n)

to avoid calculating sqrt(n) each time we can use the fact
(x+1)^2 - x^2 = 2x + 1
between 1 (1^2) and 4 (2^2) there are 3 (2.1+1) numbers i.e. 3 numbers have sqrt(n) = 1 (1,2,3)
between 4 (2^2) and 9 (3^2) there are 5 (2.2+1) numbers i.e. 5 numbers have sqrt(n) = 2 (4,5,6,7,8)
between 9 (3^2) and 16 (4^2) there are 7 (2.3+1) numbers i.e. 7 numbers have sqrt(n) = 3 (9,10,11,12,13,14,15)

so initally sqrt will be 1 (sqrtN) which will represent square roots of numbers till 3 (limN)
and that will be increased by 5 (2.2+1) next time

Alternate solution:
we can construct a Sieve of Eratosthenes size of the table will be x
where x/(log(x)-1) > 10001, the fuction is a lower bound of pi function
*/

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <cmath>

using namespace std;
typedef unsigned long natural;

const int limit = 10001;

vector<int> populatePrimes(int limit) {
	vector<int> primes;
	int sqrtLimit = sqrt(limit + 1) + 1, k = 0;
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

natural piInverse(natural n) {
	double x = 20;
	while (x / log(x) < n)
		x *= 1.5;
	return x;
}

int main() {
	// The 10,001st prime is 104,743.
	auto primes = populatePrimes(piInverse(limit));
	cout << primes[limit - 1] << endl;
}