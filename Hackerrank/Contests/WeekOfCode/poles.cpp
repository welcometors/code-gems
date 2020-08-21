#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;
using natural = uint64_t;

typedef struct {
	natural x, w;
}Pole;

natural getCost(vector<Pole>& poles, int top, int bottom) {
	natural bottomx = poles[bottom].x, cost = 0;
	for (size_t i = bottom + 1; i <= top; ++i)
		cost += poles[i].w*(poles[i].x - bottomx);
	return cost;
}

natural minCostBF(vector<Pole> poles, size_t k) {
	auto n = poles.size();
	vector<bool> selected(n - 1);
	fill(selected.end() - k + 1, selected.end(), true);
	natural minCost = -1;
	do {
		natural cost = 0;
		size_t top = n - 1;
		for (size_t i = n; --i; )
			if (selected[i-1]) {
				cost += getCost(poles, top, i);
				top = i - 1;
			}
		cost += getCost(poles, top, 0);
		if (cost < minCost)
			minCost = cost;
	} while (next_permutation(selected.begin(), selected.end()));
	return minCost;
}

natural minCost(vector<Pole> poles, size_t k) {
	auto n = poles.size();
	if (k == 1)
		return getCost(poles, n - 1, 0);

	vector<vector<natural>> costs(n, vector<natural>(k, 0));
	for (size_t i = n - 1; i >= 1; --i)
		costs[i][0] = getCost(poles, i - 1, 0);
	for (size_t j = 1; j <= k - 2; ++j) {
		for (size_t i = n - 1; i > j; --i) {
			natural minCost = -1;
			for (size_t p = i - 1; p >= j; --p) {
				natural cost = getCost(poles, i - 1, p) + costs[p][j - 1];
				if (cost < minCost)
					minCost = cost;
			}
			costs[i][j] = minCost;
		}
	}
	natural minCost = -1;
	for (size_t p = n - 1; p >= k - 1; --p) {
		natural cost = getCost(poles, n - 1, p) + costs[p][k - 2];
		if (cost < minCost)
			minCost = cost;
	}
	return minCost;
}

void test(size_t t, size_t l, natural x, natural w) {
	size_t failed = 0;
	for (size_t i = 0; i < t; ++i) {
		size_t n = 1 + (rand() % l);
		vector<Pole> poles;
		poles.reserve(n);
		natural u = 0;
		for (size_t j = 0; j < n; ++j) {
			u += 1 + (rand() % x);
			poles.emplace_back(Pole{ u, 1 + (rand() % w) });
		}
		size_t k = 1 + (rand() % n);
 		auto r1 = minCostBF(poles, k);
		auto r2 = minCost(poles, k);
		if (r1 != r2) {
			++failed;
			for (auto pole : poles)
				cout << pole.x << ":" << pole.w << ",";
			cout << '\n' << k << '\n';
			cout << r1 << " != " << r2 << '\n';
		}
	}
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	test(1000, 5, 1, 1);
	cout << "OK\n";

	size_t n, k;
	cin >> n >> k;

	vector<Pole> poles;
	poles.reserve(n);

	for (size_t i = 0, x, w; i < n; ++i, poles.emplace_back(Pole{ x, w }))
		cin >> x >> w;

	if (k == 1)
		cout << minCost(poles, k) << '\n';
	else
		cout << minCostBF(poles, k) << '\n';
}

// 3 1 20 1 30 1 40 1
// 30
// 6 2 10 15 12 17 16 18 18 13 30 10 32 1
// 216