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
#include <cstdint>
#include <vector>

using namespace std;
typedef unsigned long natural;

vector<natural> primes = { 2, 3, 5, 7 };
int totalPrimes = 4;
const int limit = 10001;

bool isPrime(natural n, int lim){
	for (int i = 2; primes[i] <= lim; i++)
		if (n % primes[i] == 0)
			return false;

	return true;
}

int main(){
	int sqrtN = 1, limN = 3;
	bool done = false;

	for (int k = 2 * 6; !done; k += 6){
		// starting from 11
		for (int i = -1; i <= 1; i += 2){
			int n = k + i;
			while (n > limN){
				sqrtN++;
				limN += 2 * sqrtN + 1;
			}

			if (isPrime(n, sqrtN)){
				primes.push_back(n);
				if (++totalPrimes == limit){
					cout << limit << "th prime is " << n << endl;
					done = true;
					break;
				}
			}
			//cout << "sqrt of " << n << " is " << sqrtN << endl;
		}
	}

	system("pause");
}