// https://projecteuler.net/problem=1
/* 
Multiples of 3 and 5
If we list all the natural numbers below 10 that are multiples of 3 or 5, we get 3, 5, 6 and 9. 
The sum of these multiples is 23.
Find the sum of all the multiples of 3 or 5 below 1000.

Solution:
Let sum of multiples of x till y is F(x)
now n = floor(y/x)
So, F(x)
= x + 2x + 3x + ... + nx
= x ( 1 + 2 + 3 + ... + n)
= x.n.(n+1)/2

using inclusion-exclusion principle
F(3) ∪ F(5) = F(3) + F(5) - F(3 ∩ 5)
			= F(3) + F(5) - F(15)
*/
namespace ProjectEuler001 {
	#include <iostream>

	using namespace std;
	typedef int natural;

	const natural limit = 999;

	natural F(natural x) {
		natural n = limit / x;
		return x*(n*(n + 1) / 2);
	}

	int main() {
		cout << F(3) + F(5) - F(15) << endl;
		system("pause");
	}
}