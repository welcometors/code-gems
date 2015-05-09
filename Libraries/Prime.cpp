using namespace std;

#pragma region "PRIME VECTOR"

#include <vector>
#include <memory>

void populatePrimes(int limit, vector<int> &primes){
	int sqrtLimit = sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset(&isPrime[2], 1, sizeof(int)*(limit - 1));
	primes.clear();

	for (int i = 2; i <= sqrtLimit; i++){
		if (!isPrime[i])
			continue;

		primes.push_back(i);

		for (int j = i + i; j <= limit; j += i)
			isPrime[j] = false;
	}

	for (int i = sqrtLimit + 1; i <= limit; i++){
		if (isPrime[i])
			primes.push_back(i);
	}
}

vector<int> populatePrimes(int limit){
	vector<int> primes;
	int sqrtLimit = sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset(&isPrime[2], 1, sizeof(int)*(limit - 1));
	primes.clear();

	for (int i = 2; i <= sqrtLimit; i++){
		if (!isPrime[i])
			continue;

		primes.push_back(i);

		for (int j = i + i; j <= limit; j += i)
			isPrime[j] = false;
	}

	for (int i = sqrtLimit + 1; i <= limit; i++){
		if (isPrime[i])
			primes.push_back(i);
	}

	return primes;
}

int totient(int n, vector<int> &primes){
	int l = 0, r = primes.size() - 1;

	while (l <= r){
		int m = (l + r) >> 1;

		if (n == primes[m])
			return m + 1;
		else if (n < primes[m])
			r = m - 1;
		else
			l = m + 1;
	}

	return l;
}

#pragma endregion