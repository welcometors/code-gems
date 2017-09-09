// https://projecteuler.net/problem=95
/*
Problem 95

Amicable chains

The proper divisors of a number are all the divisors excluding the number itself. 
For example, the proper divisors of 28 are 1, 2, 4, 7, and 14. As the sum of these 
divisors is equal to 28, we call it a perfect number.

Interestingly the sum of the proper divisors of 220 is 284 and the sum of the proper 
divisors of 284 is 220, forming a chain of two numbers. For this reason, 
220 and 284 are called an amicable pair.

Perhaps less well known are longer chains. For example, starting with 12496, 
we form a chain of five numbers:

12496 -> 14288 -> 15472 -> 14536 -> 14264 (-> 12496 -> ...)

Since this chain returns to its starting point, it is called an amicable chain.

Find the smallest member of the longest amicable chain with no element exceeding one million.

Solution:
*/

#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using natural = unsigned long;

constexpr size_t limit = 1'000'000;

vector<natural> divisorSums(limit + 1, 1);
vector<int> chainLength(limit + 1, -1);
vector<int> smallestMember(limit + 1, 0);

void calcDivisorSumTable(size_t limit) {
	for (size_t i = 2; i <= limit; i++)
		for (size_t j = i + i; j <= limit; j += i)
			divisorSums[j] += i;
}


natural getChain(natural n) {
	if (smallestMember[n])
		return smallestMember[n];

	smallestMember[n] = n;
	auto next = divisorSums[n];
	if (n == next) {
		chainLength[n] = 0;
		return n;
	}
	if (next > limit)
		return smallestMember[n] = -1;
	
	auto smallest = getChain(next);
	if (smallest < 0)
		return smallest;

	chainLength[n] = chainLength[next] + 1;
	return smallestMember[n] = smallest;
}

auto compute() {
	calcDivisorSumTable(limit);

	for (size_t i = 2; i <= limit; i++)
		getChain(i);

	int maxChain = 0, max = 0;
	for (int i = 0; i <= limit; i++) {
		if (i == smallestMember[i]) {
			if (chainLength[i] > maxChain) {
				maxChain = chainLength[i];
				max = i;
			}
			//cout << i << '\t' << chainLength[i] << '\t' << smallestMember[i] << '\t' << divisorSums[i] << '\n';
		}
	}

	return max;
}

template <class T>
inline void DoNotOptimize(const T &value) {
	__asm { lea ebx, value }
}

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	DoNotOptimize(result);
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6
		<< " miliseconds." << endl;
	cout << result << endl;
}