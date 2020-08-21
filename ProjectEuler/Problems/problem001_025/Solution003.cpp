// https://projecteuler.net/problem=3
/* 
The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143 ?
*/

#include <iostream>
#include <cstdint>

typedef uint64_t natural;

auto getMaxFactor(natural n) {
	natural max = 1;

	if (n % 2 == 0) {
		max = 2;
		do {
			n /= 2;
		} while (n % 2 == 0);
	}

	if (n % 3 == 0) {
		max = 3;
		do {
			n /= 3;
		} while (n % 3 == 0);
	}

	for (int p = 5, i = 0; 1 < n; p += (i & 1) ? 4 : 2, i++) {
		if (n % p == 0) {
			max = p;
			do {
				n /= p;
			} while (n % p == 0);
		}
	}

	return max;
}

int main(){
	using namespace std;
	cout << getMaxFactor(600851475143ULL) << '\n';
}
