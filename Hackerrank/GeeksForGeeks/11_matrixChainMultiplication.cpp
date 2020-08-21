// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-8-matrix-chain-multiplication/

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

auto matrixChainMultiplication(const vector<int>& sizes) {
	int matrices = sizes.size() - 1;
	vector<vector<int>> opt(1, vector<int>(matrices, 0));
	
	for (int len = 2; len <= matrices; len++) {
		opt.push_back({});
		for (int start = 0, end = len - 1; end < matrices; end++, start++) {
			int minCost = numeric_limits<int>::max();
			for (int partition = start; partition < end; partition++) {
				int cost = opt[partition - start][start]
					+ opt[end - partition - 1][partition + 1]
					+ sizes[start] * sizes[partition + 1] * sizes[end + 1];
				if (cost < minCost)
					minCost = cost;
			}
			opt.back().push_back(minCost);
		}
	}

	return opt.back().back();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const vector<int> sizes = { 1, 2, 3, 4, 3};
	cout << matrixChainMultiplication(sizes) << '\n';
}