// https://projecteuler.net/problem=27
/*
Quadratic primes

Euler discovered the remarkable quadratic formula:

n^2+n+41
It turns out that the formula will produce 40 primes for the consecutive integer values 0<=n<=39.
However, when n=40,40^2+40+41=40(40+1)+41 is divisible by 41, and certainly when n=41,41^2+41+41 is clearly divisible by 41.

The incredible formula n^2-79n+1601 was discovered, which produces 80 primes for the consecutive values 0<=n<=79.
The product of the coefficients, -79 and 1601, is -126479.

Considering quadratics of the form:

n^2+an+b, where |a|<1000 and |b|<1000

where |n| is the modulus/absolute value of n
e.g. |11|=11 and |-4|=4
Find the product of the coefficients, a and b, for the quadratic expression that produces the maximum number of primes 
for consecutive values of n, starting with n=0.

Solution:
b must be prime to satify condition for n=0
*/

#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <algorithm>
using namespace std;
typedef unsigned int natural;

unique_ptr<int[]> isPrime;
vector<int> populatePrimes(int limit) {
	vector<int> primes;
	int sqrtLimit = (int)sqrt(limit + 1) + 1;
	isPrime = move(unique_ptr<int[]>(new int[limit + 1]));

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

int countConsecutivePrimes(int a, int b) {
	for (int n = 0, x; ; n++)
		if ((x = n*n + a*n + b) < 0 || !isPrime[x])
			return n;
}

int main() {
	constexpr int limit = 1000;
	auto primes = populatePrimes(10000);

	int max = 0;
	pair<int, int> result;
	for (int a = -limit + 1; a < limit; a++) {
		for (int i = 0; primes[i] < limit; i++) {
			for (int k = -1; k <= 1; k += 2) {
				int n = countConsecutivePrimes(a, primes[i] * k);
				if (n > max) {
					max = n;
					result = { a, primes[i] * k };
				}
			}
		}
	}
	cout << result.first*result.second << endl;
}