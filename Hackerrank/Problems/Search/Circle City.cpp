// https://www.hackerrank.com/challenges/circle-city

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
using namespace std;

#define MAXRADIUS 2000000000
#define SQRTMAXRADIUS 44722

vector<int> getPrimes(int limit){
	vector<int> primes;
	int sqrtLimit = sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset( &isPrime[2], 1, sizeof(int)*(limit - 1));

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

vector<pair<int,int>> getPrimeFactors(int n, const vector<int> &primes){
	vector<pair<int, int>> list;

	for (auto p : primes){
		if (p > n)
			break;
		
		int c = 0;
		while (n % p == 0){
			n /= p;
			c++;
		}
		
		if (c)
			list.push_back(make_pair(p, c));
	}

	if (n > 1)
		list.push_back(make_pair(n, 1));

	return list;
}

void runForAllDivisors(const vector<pair<int, int>> &factors, const function<void(int)> &func){
	vector<int> divisors = { 1 };

	for (auto &f : factors){
		divisors.push_back(divisors.back() * (f.second + 1));
	}

	for (int i = 0, totalDivisors = divisors.back(); i < totalDivisors; i++){
		int divisor = 1;
		
		for (int j = 0, totalFactors = factors.size(); j < totalFactors; j++){
			int power = (i / divisors[j]) % (factors[j].second + 1);
			
			for (int k = 0; k < power; k++)
				divisor *= factors[j].first;
		}

		func(divisor);
	}
}

int main(){
	vector<int> primes = getPrimes(SQRTMAXRADIUS);

	int t;
	cin >> t;

	while (t--){
		int r, k;
		cin >> r >> k;

		vector<pair<int, int>> factors = getPrimeFactors(r, primes);
		int rem1mod4 = 0, rem3mod4 = 0;
		
		runForAllDivisors(factors,
			[&rem1mod4, &rem3mod4]
			(int d){
				if (d % 4 == 1)
					rem1mod4++;
				else if (d % 4 == 3)
					rem3mod4++;
			}
		);

		int minimumStations = 4 * (rem1mod4 - rem3mod4);

		cout << ((k < minimumStations) ? "impossible" : "possible ") << endl;
	}

	system("pause");
	return 0;
}

// 5 1 3 1 4 4 4 25 11 25 12