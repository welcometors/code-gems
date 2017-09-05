// https://projecteuler.net/problem=24
/*
Lexicographic permutations

A permutation is an ordered arrangement of objects.
For example, 3124 is one possible permutation of the digits 1, 2, 3 and 4.
If all of the permutations are listed numerically or alphabetically,
we call it lexicographic order. The lexicographic permutations of 0, 1 and 2 are:

012   021   102   120   201   210

What is the millionth lexicographic permutation of the digits 0, 1, 2, 3, 4, 5, 6, 7, 8 and 9?

Solution:
0123456789	999999 = 999999
2013456789	999999 = 2*9!+274239
2701345689	999999 = 2*9!+6*8!+32319
2780134569	999999 = 2*9!+6*8!+6*7!+2079
2783014569	999999 = 2*9!+6*8!+6*7!+2*6!+639
2783901456	999999 = 2*9!+6*8!+6*7!+2*6!+5*5!+39
2783910456	999999 = 2*9!+6*8!+6*7!+2*6!+5*5!+1*4!+15
2783915046	999999 = 2*9!+6*8!+6*7!+2*6!+5*5!+1*4!+2*3!+3
2783915406	999999 = 2*9!+6*8!+6*7!+2*6!+5*5!+1*4!+2*3!+1*2!+1
2783915460	999999 = 2*9!+6*8!+6*7!+2*6!+5*5!+1*4!+2*3!+1*2!+1*1!
*/

#include <iostream>
#include <string>
using namespace std;
typedef unsigned long long natural;

natural factorial[] = { 1,1,2,6,24,120,720,5040,40320,362880,3628800,
39916800,479001600,6227020800,87178291200,1307674368000,20922789888000,
355687428096000,6402373705728000,121645100408832000
};

template<class T>
T getNthPermutation(const T& s, natural n) {
	size_t l = s.length() - 1;
	T p = s;
	for (size_t i = 0; i < l; i++) {
		natural j = n / factorial[l - i];
		if (j) { // move jth value to front
			auto val = p[j + i];
			for (size_t k = j + i; k > i; k--)
				p[k] = p[k - 1];
			p[i] = val;
			n -= j * factorial[l - i];
		}
	}
	return p;
}

int main() {
	string s = "0123456789";
	cout << getNthPermutation(s, 1000000 - 1) << '\n';
}