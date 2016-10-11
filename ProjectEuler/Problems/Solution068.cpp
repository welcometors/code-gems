// https://projecteuler.net/problem=68
/*
Magic 5-gon ring

Consider the following "magic" 3-gon ring, filled with the numbers 1 to 6, and each
line adding to nine.

   4
    \
     \
      3
     / \
    /   \
   1-----2-----6
  /
 /
5

Working clockwise, and starting from the group of three with the numerically lowest
external node (4,3,2 in this example), each solution can be described uniquely.
For example, the above solution can be described by the set: 4,3,2; 6,2,1; 5,1,3.

It is possible to complete the ring with four different totals: 9, 10, 11, and 12.
There are eight solutions in total.

Total	Solution Set
    9	4,2,3; 5,3,1; 6,1,2
	9	4,3,2; 6,2,1; 5,1,3
   10	2,3,5; 4,5,1; 6,1,3
   10	2,5,3; 6,3,1; 4,1,5
   11	1,4,6; 3,6,2; 5,2,4
   11	1,6,4; 5,4,2; 3,2,6
   12	1,5,6; 2,6,4; 3,4,5
   12	1,6,5; 3,5,4; 2,4,6

By concatenating each group it is possible to form 9-digit strings;
the maximum string for a 3-gon ring is 432621513.

Using the numbers 1 to 10, and depending on arrangements, it is possible to form 16-
and 17-digit strings. What is the maximum 16-digit string for a "magic" 5-gon ring?

Solution:
Create all possible pairs of adjacent internal nodes e.g. in case of 3-gon 1-2,1-3,1-4,
1-5,1-6, 2-1,2-3,2-4,... etc.
Given the sum create outer node as sum - total of pair of internal nodes. That will
create all the possible lines for the given sum. If the outer node is zero in the line
that means that line is impossible for the given sum.

Now, run a dfs by selecting 1 internal node at a time from the possible lines. Mask can
be used to avoid duplicate. An internal mask can also be used to avoid visiting different
rotations of the same solution.

*/

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>

std::vector<std::vector<int>> lines;
int polygon, maxNumber, imask;
std::vector<int> ngon;
std::set<std::string> solutions;

void addSolution() {
	using namespace std;
	vector<vector<int>> tbl;
	for (int i = 1; i < maxNumber; i += 2)
		tbl.push_back({ ngon[i], ngon[i - 1], ngon[(i + 1 < maxNumber) ? (i + 1) : 0] });

	auto idx = min_element(begin(tbl), end(tbl), [](const auto& a1, const auto& a2) {
		return a1[0] < a2[0];
	}) - begin(tbl);

	string rep;
	for (int i = 0; i < polygon; i++, idx = (idx + 1) % polygon)
		rep += to_string(tbl[idx][0]) + to_string(tbl[idx][1]) + to_string(tbl[idx][2]);
	solutions.insert(rep);
}

void dfs(int mask, int idx) {
	if (idx == maxNumber - 2) {
		const auto k = lines[ngon[idx]][ngon[0]];
		if (k && !(mask&(1 << k))) {
			ngon[idx + 1] = k;
			addSolution();
		}
		return;
	}

	for (int i = 1; i <= maxNumber; ++i) {
		if (!(mask & (1 << i)) && !(imask & (1 << i))) {
			const auto k = lines[ngon[idx]][i];
			if (k && !(mask&(1 << k))) {
				ngon[idx + 1] = k;
				ngon[idx + 2] = i;
				dfs(mask | (1 << i) | (1 << k), idx + 2);
			}
		}
	}
}

void generate(int sum) {
	solutions.clear();
	lines.clear();
	for (int i = 0; i <= maxNumber; ++i) {
		std::vector<int> line = { 0 };
		for (int j = 1; j <= maxNumber; ++j) {
			const int k = sum - i - j;
			line.push_back((k > 0 && k <= maxNumber && k != i && k != j) ? k : 0);
		}
		lines.emplace_back(line);
	}

	imask = 0;
	for (int i = 1; i <= polygon + 1; ++i) {
		ngon[0] = i;
		imask |= 1 << i;
		dfs(1 << i, 0);
	}
}

auto compute() {
	polygon = 5;
	maxNumber = 2 * polygon;
	ngon.resize(maxNumber);

	generate(14);
	std::string max;
	for (const auto& rep : solutions)
		if (rep.length() == 16 && rep > max)
			max = rep;
	return max;
}

int main() {
	using namespace std;
	using namespace chrono;
	auto start = high_resolution_clock::now();
	auto result = compute();
	cout << "Done in "
		<< duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / 1000000.0
		<< " miliseconds." << endl;
	cout << result << endl;
}