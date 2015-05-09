// https://projecteuler.net/problem=4
/* 
A palindromic number reads the same both ways. The largest palindrome made from the product 
of two 2-digit numbers is 9009 = 91 × 99.

Find the largest palindrome made from the product of two 3-digit numbers.

Solution:
let palindrome no be abccba
abccba = 10^5.a + 10^4.b + 10^3.c + 10^2.c + 10.b + a
	   = 100001.a + 10010.b + 1100.c
	   = 11 ( 9091.a + 910.b + 100.c )

so one of them has to be multiple of 11
*/

#include <iostream>
#include <cstdint>

using namespace std;
const int uLimit = 999, lLimit = 100;

template <typename T>
bool isPalindrome(T n){
	T r = 0, c = n;
	while (c){
		r = r * 10 + c % 10;
		c /= 10;
	}
	return r == n;
}

int main(){
	int max = 0;

	for (int a = uLimit; a >= lLimit; a--){
		if (a*a < max)
			break;

		int step = (a % 11 == 0) ? 1 : 11;
		int start = (a % 11 == 0) ? a : 11 * (a / 11);

		for (int b = start; b >= lLimit; b -= step){
			int m = a*b;
			if (m <= max)
				break;
			if (isPalindrome<int>(m))
				max = m;
		}
	}

	cout << max << endl;
	system("pause");
}