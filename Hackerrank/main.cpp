#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define sort(x) sort(x.begin(), x.end())
#define input(type, var) type var; cin >> var

#include <memory>
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

int main() {
	std::ios::sync_with_stdio(false);
	cin.tie(0);

	//The 10,000th prime is 104,729.
	auto primes = populatePrimes(104730);

	input(int, t);
	while (t--) {
		input(int, n);
		cout << primes[n] << endl;
	}

#if _MSC_VER >= 1600
	system("pause");
#endif
}