// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-6-min-cost-path/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using matrix = vector<vector<int>>;

auto minCostPath(const matrix& mat) {
	int rows = mat.size(), columns = mat.front().size();
	matrix cost(rows, vector<int>(columns));

	cost[0][0] = mat[0][0];

	for (int j = 1; j < rows; j++)
		cost[j][0] = mat[j][0] + cost[j - 1][0];

	for (int i = 1; i < columns; i++)
		cost[0][i] = mat[0][i] + cost[0][i - 1];

	for (int j = 1; j < rows; j++)
		for (int i = 1; i < columns; i++)
			cost[j][i] = min({ cost[j - 1][i], cost[j - 1][i - 1], cost[j][i - 1] }) + mat[j][i];

	return cost.back().back();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const matrix mat = {
		{ 1, 2, 3 },
		{ 4, 8, 2 },
		{ 1, 5, 3 } };

	cout << minCostPath(mat) << '\n';
}