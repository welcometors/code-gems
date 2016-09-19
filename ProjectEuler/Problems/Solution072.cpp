// https://projecteuler.net/problem=72
/*
Ordered fractions

Consider the fraction, n/d, where n and d are positive integers. If n<d and HCF(n,d)=1, it is called a reduced proper fraction.

If we list the set of reduced proper fractions for d <= 8 in ascending order of size, we get:

1/8, 1/7, 1/6, 1/5, 1/4, 2/7, 1/3, 3/8, 2/5, 3/7, 1/2, 4/7, 3/5, 5/8, 2/3, 5/7, 3/4, 4/5, 5/6, 6/7, 7/8

It can be seen that there are 21 elements in this set.

How many elements would be contained in the set of reduced proper fractions for d <= 1,000,000?

Solution:
The form n/d can not be reduced if n and d are co-prime. So, for all the co-primes xi <= d, the form
xi/d can not be reduced. Number of co-primes below d is phi(d) [totient function].

It can be directly computed as  2<=n<=1000000 sigma phi(n)
Or
[1<=k<=n]sigma[phi(k)] = 1/2(1 + [1<=k<=n]sigma[mu(k)*floor(n/k)^2])
where mu is mobius function
*/

#include <iostream>
#include <cmath>
#include <chrono>
using namespace std;

#include <memory>
#include <cstring>

unique_ptr<int[]> populateMobius(int limit) {
	int sqrtLimit = (int)sqrt(limit) + 1;
	unique_ptr<int[]> sieve(new int[limit + 1]);
	for (int i = 1; i <= limit; i++)
		sieve[i] = 1;

	for (int i = 2; i <= sqrtLimit; i++) {
		if (sieve[i] == 1) {
			for (int j = i; j <= limit; j += i)
				sieve[j] *= -i;
			for (int j = i * i; j <= limit; j += i * i)
				sieve[j] = 0;
		}
	}

	for (int i = 2; i <= limit; i++)
		if (sieve[i] == i)
			sieve[i] = 1;
		else if (sieve[i] == -i)
			sieve[i] = -1;
		else if (sieve[i] < 0)
			sieve[i] = 1;
		else if (sieve[i] > 0)
			sieve[i] = -1;

	return sieve;
}

long long compute1() {
	long long n = 1000000;
	auto mobius = populateMobius(n);

	long long sum = 1;
	for (int k = 1; k <= n; k++)
		if (mobius[k]) {
			long long d = n / k;
			d *= d;
			sum += mobius < 0 ? -d : d;
		}
	return sum / 2 - 1;
}

unique_ptr<int[]> populatePhi(int limit) {
	int sqrtLimit = (int)sqrt(limit) + 1;

	unique_ptr<int[]> sieve(new int[limit + 1]);
	sieve[0] = 1;
	for (int i = 1; i <= limit; i++)
		sieve[i] = i;

	for (int i = 2; i <= limit; i++)
		if (sieve[i] == i)
			for (int j = i; j <= limit; j += i)
				sieve[j] -= sieve[j] / i;

	return sieve;
}

long long compute2() {
	long long n = 1000000;
	auto phi = populatePhi(n);

	long long sum = 0;
	for (int k = 2; k <= n; k++)
		sum += phi[k];
	return sum;
}

int main() {
	auto begin = chrono::high_resolution_clock::now();
	auto result = compute2();
	cout << "Done in "
		<< chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - begin).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}