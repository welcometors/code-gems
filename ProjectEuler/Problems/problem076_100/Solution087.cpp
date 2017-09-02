// https://projecteuler.net/problem=87
/*
Problem 87

Prime power triples

The smallest number expressible as the sum of a prime square, prime cube, 
and prime fourth power is 28. In fact, there are exactly four numbers below 
fifty that can be expressed in such a way:

28 = 2^2 + 2^3 + 2^4
33 = 3^2 + 2^3 + 2^4
49 = 5^2 + 2^3 + 2^4
47 = 2^2 + 3^3 + 2^4

How many numbers below fifty million can be expressed as the sum of a prime 
square, prime cube, and prime fourth power?

Solution:

*/

#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <algorithm>
#include <chrono>
using namespace std;


auto populatePrimes(int limit) {
	vector<unsigned> primes;
	int sqrtLimit = (int)sqrt(limit + 1) + 1;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	fill_n(&isPrime[2], limit - 1, 1);
	primes.clear();

	for (int i = 2; i <= sqrtLimit; i++)
		if (isPrime[i]) {
			primes.push_back(i);
			for (int j = i + i; j <= limit; j += i)
				isPrime[j] = false;
		}

	for (int i = sqrtLimit + 1; i <= limit; i++)
		if (isPrime[i])
			primes.push_back(i);

	return primes;
}

auto compute() {
	constexpr int limit = 50'000'000;
	auto primes = populatePrimes(sqrt(limit) + 1);

	unique_ptr<int[]> done(new int[limit + 1]);
	fill_n(done.get(), limit + 1, 0);
	int ctr = 0;

	for (auto& p1 : primes) {
		int c = p1*p1*p1*p1;
		if (c >= limit)
			break;
		for (auto& p2 : primes) {
			int b = c + p2*p2*p2;
			if (b >= limit)
				break;
			for (auto& p3 : primes) {
				int a = b + p3*p3;
				if (a >= limit)
					break;
				if (!done[a]) {
					done[a] = 1;
					ctr++;
				}
			}
		}
	}

	return ctr;
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