// https://projecteuler.net/problem=74
/*
Digit factorial chains

The number 145 is well known for the property that the sum of the factorial of its digits is equal to 145:

1! + 4! + 5! = 1 + 24 + 120 = 145

Perhaps less well known is 169, in that it produces the longest chain of numbers that link back to 169;
it turns out that there are only three such loops that exist:

169 -> 363601 -> 1454 -> 169
871 ->  45361 ->  871
872 ->  45362 ->  872

It is not difficult to prove that EVERY starting number will eventually get stuck in a loop.
For example,
69 -> 363600 -> 1454 -> 169 -> 363601 (-> 1454)
78 -> 45360 -> 871 -> 45361 (-> 871)
540 -> 145 (-> 145)

Starting with 69 produces a chain of five non-repeating terms, but the longest non-repeating chain with
a starting number below one million is sixty terms.

How many chains, with a starting number below one million, contain exactly sixty non-repeating terms?

Solution:
*/

#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <chrono>
using namespace std;

const int factorial[] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880 };
vector<int> lengthMap;

template<class T>
T digitFactorialSum(T n) {
	if (n == 0)
		return factorial[0];
	T sum = 0;
	while (n) {
		sum += factorial[n % 10];
		n /= 10;
	}
	return sum;
}

auto getLength(int n) {
	auto sum = digitFactorialSum(n);
	if (sum == n)
		return lengthMap[n] = 1;
	if (lengthMap[sum])
		return lengthMap[sum] + 1;
	return lengthMap[n] = getLength(sum) + 1;
}

void init(int n) {
	lengthMap.resize(n, 0);
	lengthMap[169] = lengthMap[363601] = lengthMap[1454] = 3;
	lengthMap[871] = lengthMap[45361] = 2;
	lengthMap[872] = lengthMap[45362] = 2;
}

auto compute() {
	constexpr int limit = 1'000'000;
	init(6 * factorial[9] + 1);
	int count = 0;
	for (int i = 0; i < limit; ++i) {
		if (!lengthMap[i]) {
			lengthMap[i] = getLength(i);
			if (lengthMap[i] == 60)
				++count;
		}
	}
	return count;
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