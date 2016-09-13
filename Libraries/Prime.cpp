using namespace std;

#pragma region "PRIME VECTOR"

#include <vector>
#include <memory>
#include <cstring>

void populatePrimes(int limit, vector<int> &primes){
	int sqrtLimit = (int)sqrt(limit + 1) + 1, k = 0;
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
	int sqrtLimit = (int)sqrt(limit + 1) + 1, k = 0;
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

#pragma region "Factors"

const unsigned primes[] = { // some primes
	2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,  41,
	43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97, 101,
	103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
	173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
	241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313 };

template<class T>
T getNumOfFactors(T n) {
	T factors = 1;
	for (auto prime : primes) {
		natural power = 0;
		while (n % prime == 0) {
			n /= prime;
			power++;
		}
		if (power)
			factors *= power + 1;
		if (n == 1)
			break;
	}
	if (n > 1)
		factors *= 2;
	return factors;
}

template<class T>
vector<T> getAllFactors(T n) {
	vector<T> factors = { 1 };
	for (auto prime : primes) {
		T power = 1;
		size_t nFactors = factors.size();
		while (n % prime == 0) {
			power *= prime;
			for (size_t i = 0; i < nFactors; i++)
				factors.push_back(factors[i] * power);
			n /= prime;
		}
		if (n == 1)
			break;
	}
	if (n > 1)
		for (size_t i = 0, nFactors = factors.size(); i < nFactors; i++)
			factors.push_back(factors[i] * n);
	return factors;
}

#pragma endregion