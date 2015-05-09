// https://projecteuler.net/problem=5
/* 
Smallest multiple

2520 is the smallest number that can be divided by each of the numbers from 1 to 10 without any remainder.

What is the smallest positive number that is evenly divisible by all of the numbers from 1 to 20?

Solution:
let the solution be n
let S is the set of all primes less than 20
then for each element s of S there will be floor(log(20 base s)) appearences of s in prime facterization of n
*/

#include <iostream>
#include <cmath>
#include <cstdint>

using namespace std;
typedef unsigned long long natural;
const int limit = 20;

int main(){
	int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19 };
	double logN = log(limit);
	natural n = 1;

	for (auto p : primes){
		n *= pow(p, (int)(logN / log(p)));
	}

	cout << n << endl;
	system("pause");
}