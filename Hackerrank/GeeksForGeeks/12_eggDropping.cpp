// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-11-egg-dropping-puzzle/

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;

auto eggDropping(int floors, int eggs) {
	vector<vector<int>> opt(eggs + 1, vector<int>(floors + 1, 1));
	for (int i = 1; i <= floors; i++)
		opt[1][i] = i;

	for (int e = 2; e <= eggs; e++) {
		for (int f = 2; f <= floors; f++) {
			int minCost = numeric_limits<int>::max();
			for (int i = 2; i <= f; i++) {
				int cost = max(opt[e - 1][i - 1], opt[e][f - i]);
				if (cost < minCost)
					minCost = cost;
			}
			opt[e][f] = minCost + 1;
		}
	}

	return opt.back().back();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	cout << eggDropping(36, 2) << '\n';
}