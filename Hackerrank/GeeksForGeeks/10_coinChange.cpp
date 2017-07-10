// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-7-coin-change/

#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
using natural = unsigned long long;

struct CantorHash {
	size_t operator()(const std::pair<int, int>& p) const {
		return (p.first + p.second)*(p.first + p.second + 1) / 2 + p.second;
	}
};

unordered_map<pair<int, int>, natural, CantorHash> ways;

natural coinChange(const vector<int>& coins, int price, int index) {
	if (price < coins.front() || index < 0)
		return 0;

	natural w = ways[{ price, index }];
	if (w)
		return w;

	for (int i = index; i >= 0; i--)
		for (int remain = price - coins[i]; remain >= 0; remain -= coins[i])
			w += remain ? coinChange(coins, remain, i - 1) : 1;

	return ways[{ price, index }] = w;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const vector<int> coins = { 1, 2, 3 };
	cout << coinChange(coins, 4, coins.size() - 1) << '\n';
}