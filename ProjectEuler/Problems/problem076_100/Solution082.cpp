// https://projecteuler.net/problem=82
/*
Path sum: three ways

The minimal path sum in the 5 by 5 matrix below, by starting in any cell in 
the left column and finishing in any cell in the right column, and only 
moving up, down, and right, is indicated in red and bold; the sum is equal 
to 994.

 131   673  [234] [103]  [18]
[201]  [96] [342]  965   150
 630   803   746   422   111
 537   699   497   121   956
 805   732   524    37   331

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
using matrixRow = vector<natural>;
using matrix = vector<matrixRow>;

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
	matrixRow dSum(n), uSum(n);
	for (int c = n - 2; c >= 0; c--) {
		uSum[0] = x[0][c] + x[0][c + 1];
		dSum[n - 1] = x[n - 1][c] + x[n - 1][c + 1];
		for (int i = 1, j = n - 2; i < n; i++, j--) {
			uSum[i] = x[i][c] + min(uSum[i - 1], x[i][c + 1]);
			dSum[j] = x[j][c] + min(dSum[j + 1], x[j][c + 1]);
		}
		for (int i = 0; i < n; i++)
			x[i][c] = min(uSum[i], dSum[i]);
	}

	natural min = x[0][0];
	for (int i = 1; i < n; i++)
		if (min > x[i][0])
			min = x[i][0];
	return min;
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