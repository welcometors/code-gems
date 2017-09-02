// https://projecteuler.net/problem=88
/*
Problem 88

Product-sum numbers

A natural number, N, that can be written as the sum and product of a given set of 
at least two natural numbers, {a1, a2, ... , ak} is called a product-sum number: 
N = a1 + a2 + ... + ak = a1 × a2 × ... × ak.

For example, 6 = 1 + 2 + 3 = 1 × 2 × 3.

For a given set of size, k, we shall call the smallest N with this property a 
minimal product-sum number. The minimal product-sum numbers for sets of size, 
k = 2, 3, 4, 5, and 6 are as follows.

k=2: 4 = 2 × 2 = 2 + 2
k=3: 6 = 1 × 2 × 3 = 1 + 2 + 3
k=4: 8 = 1 × 1 × 2 × 4 = 1 + 1 + 2 + 4
k=5: 8 = 1 × 1 × 2 × 2 × 2 = 1 + 1 + 2 + 2 + 2
k=6: 12 = 1 × 1 × 1 × 1 × 2 × 6 = 1 + 1 + 1 + 1 + 2 + 6

Hence for 2 <= k <= 6, the sum of all the minimal product-sum numbers is 4+6+8+12 = 30; 
note that 8 is only counted once in the sum.

In fact, as the complete set of minimal product-sum numbers for 2 <= k <= 12 is 
{4, 6, 8, 12, 15, 16}, the sum is 61.

What is the sum of all the minimal product-sum numbers for 2 <= k <= 12000?

Solution:

for a given k, minimal product-sum number can't be greater than 2k as we can make
1 × 1 × ... (k-2 times) × 2 × k = 1 + 1 + ... (k-2 times) + 2 + k

after that go through every combination
2*2, 2*3, 2*4, ... 2*6000
3*4, 3*5, 3*6, ... 
	... sqrt(12000)*sqrt(12000)
now,
2*2*2, 2*2*3, ... and so on

for each sunch combination find 'k' and update if it's minimum
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <chrono>
using namespace std;


int log2(int x) {
	int v = 0;
	for (; x; x >>= 1)
		v++;
	return v;
}

constexpr int limit = 12000;

void propogate(vector<int>& mins, int start, int product, int sum, int idx, int max) {
	if (idx == max)
		return;

	for (int v = start, s = start + sum, p = start*product; p <= 2 * limit; v++, s++, p += product) {
		int k = p - s + idx + 1;
		if (k > limit)
			break;
		if (mins[k] > p)
			mins[k] = p;
		propogate(mins, v, p, s, idx + 1, max);
	}
}

auto compute() {
	vector<int> mins(limit + 1, 2*limit);
	int max = sqrt(2 * limit);
	int slots = log2(2 * limit);

	for (int k = 2; k <= max; k++)
		propogate(mins, k, k, k, 1, slots);

	set<int> unique(mins.begin() + 2, mins.end());
	return accumulate(unique.begin(), unique.end(), 0);
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