// https://projecteuler.net/problem=6
/* 
Sum square difference

The sum of the squares of the first ten natural numbers is,

1^2 + 2^2 + ... + 10^2 = 385
The square of the sum of the first ten natural numbers is,

(1 + 2 + ... + 10)^2 = 55^2 = 3025
Hence the difference between the sum of the squares of the first ten natural numbers 
and the square of the sum is 3025 − 385 = 2640.

Find the difference between the sum of the squares of the first one hundred natural 
numbers and the square of the sum.

Solution

1 + 2 + 3 + ... + n = n(n+1)/2
1^2 + 2^2 + ... + n^2 = n(n+1)(2n+1)/6

(1 + 2 + 3 + ... + n)^2 - (1^2 + 2^2 + ... + n^2)
= (n(n+1)/2)^2 - n(n+1)(2n+1)/6
= (n(n+1)/2).((3n^2-n-2)/6)
*/

#include <iostream>
#include <cstdint>

using namespace std;
typedef unsigned long long natural;
const int n = 100;

int main(){
	natural a = n*(n + 1) / 2;
	natural b = (3 * n*n - n - 2) / 6;

	cout << a*b << endl;
	system("pause");
}