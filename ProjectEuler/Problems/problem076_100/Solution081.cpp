// https://projecteuler.net/problem=81
/*
Path sum: two ways

In the 5 by 5 matrix below, the minimal path sum from the top left to the 
bottom right, by only moving to the right and down, is indicated in bold 
red (here square braces) and is equal to 2427.

[131]  673   234   103    18
[201]  [96] [342]  965   150
 630   803  [746] [422]  111
 537   699   497  [121]  956
 805   732   524   [37] [331]

Find the minimal path sum, in matrix.txt (right click and "Save Link/Target As..."), 
a 31K text file containing a 80 by 80 matrix, from the top left to the bottom 
right by only moving right and down.

Solution:


*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;
using natural = unsigned long long;
using matrix = vector<vector<natural>>;

auto fastSplit(const string &s) {
	vector<natural> tokens;
	size_t start = 0, length = 0;

	for (auto c : s) {
		if (c == ',') {
			if (length == 0) {
				start++;
				continue;
			}
			tokens.push_back(stoi(s.substr(start, length)));
			start += length + 1;
			length = 0;
		}
		else
			length++;
	}

	if (length != 0)
		tokens.push_back(stoi(s.substr(start, length)));

	return tokens;
}

auto getMatrix(const string &fileName) {
	matrix x;
	std::ifstream file(fileName);
	if (file.good()) {
		while (!file.eof()) {
			string row;
			file >> row;
			auto elems = fastSplit(row);
			if(elems.size())
				x.emplace_back(elems);
		}
	}
	else {
		cout << "File not found. \n";
	}
	return x;
}

auto getMinimumSum(matrix& x) {
	const int n = x.size();
	for (int i = n - 2; i >= 0; i--) {
		x[n - 1][i] += x[n - 1][i + 1];
		x[i][n - 1] += x[i + 1][n - 1];
	}
	for (int i = n - 2; i >= 0; i--)
		for (int j = n - 2; j >= 0; j--)
			x[i][j] += min(x[i + 1][j], x[i][j + 1]);
	return x.front().front();
}

auto compute() {
	/*matrix x = {
		{ 131, 673, 234, 103,  18 },
		{ 201,  96, 342, 965, 150 },
		{ 630, 803, 746, 422, 111 },
		{ 537, 699, 497, 121, 956 },
		{ 805, 732, 524,  37, 331 }
	};
	return getMinimumSum(x);*/
	return getMinimumSum(getMatrix(".\\Data\\p081_matrix.txt"));
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