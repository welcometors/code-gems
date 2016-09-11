// https://projecteuler.net/problem=23
/*
Non-abundant sums

A perfect number is a number for which the sum of its proper divisors is exactly equal to the number. 
For example, the sum of the proper divisors of 28 would be 1 + 2 + 4 + 7 + 14 = 28, 
which means that 28 is a perfect number.

A number n is called deficient if the sum of its proper divisors is less than n and it is called abundant 
if this sum exceeds n.

As 12 is the smallest abundant number, 1 + 2 + 3 + 4 + 6 = 16, the smallest number that can be written 
as the sum of two abundant numbers is 24. By mathematical analysis, it can be shown that all integers 
greater than 28123 can be written as the sum of two abundant numbers. However, this upper limit cannot 
be reduced any further by analysis even though it is known that the greatest number that cannot be expressed 
as the sum of two abundant numbers is less than this limit.

Find the sum of all the positive integers which cannot be written as the sum of two abundant numbers.
*/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef unsigned int natural;
constexpr natural limit = 28123;
const natural primes[] = { //sqrt(28123) = 167
	2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,  41,
	43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97, 101,
	103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167 };

natural getFactorsSum(natural n) {
	natural factorsSum = 1;
	for (auto prime : primes) {
		natural power = prime, powerSum = 0;
		while (n % prime == 0) {
			n /= prime;
			powerSum += power;
			power *= prime;
		}
		if(powerSum)
			factorsSum *= powerSum + 1;
		if (n == 1)
			break;
	}
	if (n > 1)
		factorsSum *= n + 1;
	return factorsSum;
}

vector<int> getAbundantNumbers() {
	vector<int> numbers;
	for (int i = 1; i <= limit; i++)
		if (getFactorsSum(i) - i > i)
			numbers.push_back(i);
	return numbers;
}

bool check(vector<int>& table, int n) {
	for (int i = 1; i < n; i++)
		if (binary_search(table.begin(), table.end(), i) && binary_search(table.begin(), table.end(), n - i)) 
			return true;
	return false;
}

int main() {
	int sum = 0;
	auto table = getAbundantNumbers();
	for (int i = 1; i <= limit; i++)
		if (!check(table, i))
			sum += i;
	cout << sum << endl;
}
