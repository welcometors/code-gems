// https://www.hackerrank.com/challenges/red-john-is-back

#include <iostream>
#include <cstring>
#include <cmath>
#include <vector>
#include <memory>
#include <unordered_map>
using namespace std;

#define natural int

void populatePrimes(int limit, vector<int> &primes){
	int sqrtLimit = sqrt(limit + 1) + 1, k = 0;
	unique_ptr<int[]> isPrime(new int[limit + 1]);

	isPrime[0] = isPrime[1] = 0;
	std::memset( &isPrime[2], 1, sizeof(int)*(limit - 1));
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

natural nCr(natural n, natural r){
	if (n < 2 || r < 1 || r == n)
		return 1;

	if (r > n - r)
		r = n - r;

	natural p = n--;
	for (int i = 2; i <= r; i++, n--)
		p = (p *  n) / i;

	return p;
}

int main(){
	vector<int> primes;
	populatePrimes(1000000, primes);
	
	int t;
	cin >> t;

	while (t--){
		int n;
		cin >> n;

		natural w = 1;
		for (int i = 1; 4 * i <= n; i++){
			w += nCr(n - 3 * i, i);
		}

		cout << totient(w, primes) << endl;
	}

	return 0;
}

/*
Consider - as a stack of 4 horizontal bars.
then n bars can be arranged as

|||||||||||||||| ( all vertical)
1 way

||-||||||||||||| ( 1 stack of 4 and others vertical)
no of ways = no of ways for solution of x + y = n - 4
where x are no of bricks in left and y are no of bricks in right
nCr( n - 4 + 2 - 1, 2 - 1 ) ways

||-||||||-|||||| ( 2 stack of 4 and others vertical)
no of ways = no of ways for solution of x + y + z = n - 8
nCr( n - 8 + 3 - 1, 3 - 1 ) ways

similarily for k stacks
nCr( n - k*4 + k+1 -1, k+1 -1 ) = nCr( n - 3*k, k)

*/