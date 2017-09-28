// https://projecteuler.net/problem=90
/*
Problem 90

Cube digit pairs

Each of the six faces on a cube has a different digit (0 to 9) written on it;
the same is done to a second cube. By placing the two cubes side-by-side in different
positions we can form a variety of 2-digit numbers.

For example, the square number 64 could be formed:

   +--------+ +--------+
  /        /|/        /|
 /        / /        / |
+--------+ +--------+  |
|        | |        |  +
|   6    | |   4    | /
|        |/|        |/
+--------+ +--------+

In fact, by carefully choosing the digits on both cubes it is possible to display all
of the square numbers below one-hundred: 01, 04, 09, 16, 25, 36, 49, 64, and 81.

For example, one way this can be achieved is by placing {0, 5, 6, 7, 8, 9} on one cube
and {1, 2, 3, 4, 8, 9} on the other cube.

However, for this problem we shall allow the 6 or 9 to be turned upside-down so that
an arrangement like {0, 5, 6, 7, 8, 9} and {1, 2, 3, 4, 6, 7} allows for all nine square
numbers to be displayed; otherwise it would be impossible to obtain 09.

In determining a distinct arrangement we are interested in the digits on each cube,
not the order.

{1, 2, 3, 4, 5, 6} is equivalent to {3, 6, 4, 1, 2, 5}
{1, 2, 3, 4, 5, 6} is distinct from {1, 2, 3, 4, 5, 9}

But because we are allowing 6 and 9 to be reversed, the two distinct sets in the
last example both represent the extended set {1, 2, 3, 4, 5, 6, 9} for the purpose
of forming 2-digit numbers.

How many distinct arrangements of the two cubes allow for all of the square numbers
to be displayed?

Solution:
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
using namespace std;

auto cubeConfigs() {
	vector<bool> digits(10, true);
	fill(digits.begin(), digits.begin() + 4, false);

	vector<int> configs;
	configs.reserve(10 * 9 * 8 * 7 / (1 * 2 * 3 * 4));	// 10C6

	do {
		int d = 0;
		for (int i = 0; i < 10; i++)
			if (digits[i])
				d |= 1 << i;

		if (d & (1 << 6))
			d |= 1 << 9;
		else if (d & (1 << 9))
			d |= 1 << 6;

		configs.push_back(d);
	} while (next_permutation(digits.begin(), digits.end()));

	return configs;
}

inline bool check(int c1, int c2, int d) {
	const int a = 1 << (d / 10), b = 1 << (d % 10);
	return ((c1 & a) && (c2 & b))
		|| ((c1 & b) && (c2 & a));
}

auto compute() {
	const int squares[] = { 1,4,9,16,25,36,49,64,81 };
	auto d = cubeConfigs();
	int count = 0, mask = 0;

	for (int s : squares)
		for (int i = 0; i < 2; i++, s /= 10)
			mask |= 1 << (s % 10);

	for (int i = 0, n = d.size(); i < n; i++) {
		for (int j = i; j < n; j++) {
			if (((d[i] | d[j]) & mask) != mask)
				continue;

			count++;
			for (auto& s : squares)
				if (!check(d[i], d[j], s)) {
					count--;
					break;
				}
		}
	}

	return count;
}

template<typename Function, class ... Types>
decltype(auto) timeit(Function f, Types ... args) {
    using namespace chrono;
    auto start = high_resolution_clock::now();
    auto result = f(args...);
    double duration = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1e6;
    return std::make_pair(result, duration);
}

int main() {
    using namespace std;
    auto[result, time] = timeit(compute);
    cout << result << " Calculated in " << time << " miliseconds." << '\n';
    return 0;
}