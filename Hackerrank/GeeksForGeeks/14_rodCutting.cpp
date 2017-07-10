// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-13-cutting-a-rod/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

auto rodCutting(const vector<int>& price) {
	int n = price.size();
	vector<int> opt(1, 0);

	for (int l = 1; l <= n; l++) {
		int maxCost = numeric_limits<int>::min();
		for (int i = 0; i < price.size() && i < l; i++) {
			int cost = price[i] + opt[l - i - 1];
			if (cost > maxCost)
				maxCost = cost;
		}
		opt.push_back(maxCost);
	}

	return opt.back();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const vector<int> ary = { 1, 5, 8, 9, 10, 17, 17, 20 };
	cout << rodCutting(ary) << '\n';
}