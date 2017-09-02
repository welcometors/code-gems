// https://projecteuler.net/problem=92
/*
Problem 92

A number chain is created by continuously adding the square of the digits in a number
to form a new number until it has been seen before.

For example,

44 -> 32 -> 13 -> 10 -> 1 -> 1
85 -> 89 -> 145 -> 42 -> 20 -> 4 -> 16 -> 37 -> 58 -> 89

Therefore any chain that arrives at 1 or 89 will become stuck in an endless loop.
What is most amazing is that EVERY starting number will eventually arrive at 1 or 89.

How many starting numbers below ten million will arrive at 89?

Solution:
for given 10^p the maximum sum can be S=9*9*p
now we can compute the ending term for first S numbers.
Now, for each x belong to S. we need to compute
how many 'p' digit numbers are there such that their sum is 'x' (x varies from 1 to S).

Let the number of ways of writing n as the sum of k squares be f(n,k).
Then f can be computed by the recurrence relation:
f(n,k) = f(n-0^2,k-1) + f(n-1^2,k-1) + f(n-2^2,k-1) + ... + f(n-9^2,k-1)

after that we can use all the sums in which ending is 89.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

int ending(int n, vector<int>& table) {
	if (table[n])
		return table[n];

	int s = 0;
	for (int x = n; x; x /= 10)
		s += (x % 10)*(x % 10);
	return table[n] = ending(s, table);
}

vector<int> table;
void genTable(int p10) {
	const int n = 81 * p10;
	table = vector<int>(max(n + 1, 1000), 0);
	table[1] = 1;
	table[89] = 89;

	for (int i = 2; i <= n; i++)
		ending(i, table);
}

auto calcV1(int p10) {
	genTable(p10);
	int p = 1;
	for (int i = 0; i < p10; i++)
		p *= 10;

	int c = 0;
	for (int n = 1; n < p; n++) {
		int s = 0;
		for (int x = n; x; x /= 10)
			s += (x % 10)*(x % 10);
		if (table[s] == 89)
			c++;
	}
	return c;
}

// can be used to calculate for 10^1000 under a second on i7 2.5GHz
auto calcV2(int p10) {
	const int n = 81 * p10;
	vector<int> m(n + 1, 0), m1(n + 1);
	for (int i = 0; i < 10; i++)
		m[i*i] = 1;

	for (int d = 2, l = 81 * 2; d <= p10; d++, l += 81) {
		for (int s = 0; s <= l; s++) {
			int k = 0;
			for (int s_i2 = s, sd = 1; sd <= 19 && s_i2 >= 0; s_i2 -= sd, sd += 2)
				k += m[s_i2];	// m[s - i*i] for 0<=i<=9
			m1[s] = k;
		}
		swap(m, m1);
	}

	genTable(p10);
	int c = 0;
	for (int s = 0; s <= n; s++)
		if (table[s] == 89)
			c += m[s];
	return c;
}

auto compute() {
	constexpr size_t limit = 7;
	return calcV2(limit);
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