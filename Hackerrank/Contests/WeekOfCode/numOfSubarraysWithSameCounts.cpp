// Author	: Rahul Sharma <rahulsrma26@gmail.com>

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include <random>
using namespace std;

int numOfSubarraysWithSameCountsBF(const vector<int>& ary, int x, int y) {
	const int n = ary.size();
	if (x == y)
		return n * (n + 1) / 2;

	int count = 0;
	for (int s = 0; s < n; s++) {
		for (int e = s; e < n; e++) {
			int xc = 0, yc = 0;
			for (int i = s; i <= e; i++) {
				if (ary[i] == x)
					xc++;
				else if (ary[i] == y)
					yc++;
			}
			if (xc == yc)
				count++;
		}
	}

	return count;
}

int numOfSubarraysWithSameCounts(const vector<int>& ary, int x, int y) {
	if (x > y)
		swap(x, y);

	const int n = ary.size();
	if (x == y)
		return n * (n + 1) / 2;

	int count = 0;
	vector<int> added(n, 0);
	stack<int> freeX, freeY;

	for (int i = 0; i < n; i++) {
		if (ary[i] == x) {
			freeX.push(i);
			if (freeY.size()) {
				if (freeY.top() > 0)
					added[i] += added[freeY.top() - 1];
				freeY.pop();
				added[i]++;
			}
		}
		else if (ary[i] == y) {
			freeY.push(i);
			if (freeX.size()) {
				if (freeX.top() > 0)
					added[i] += added[freeX.top() - 1];
				freeX.pop();
				added[i]++;
			}
		}
		else {
			if (i > 0)
				added[i] += added[i - 1];
			added[i]++;
		}
		count += added[i];
	}

	return count;
}

void test(int total, int maxSize, int maxValue) {
	default_random_engine eng;
	uniform_int_distribution<int> arraySize(1, maxSize);
	uniform_int_distribution<int> arrayValue(1, maxValue);

	int failed = 0;
	for (int i = 0; i < total; i++) {
		vector<int> ary(arraySize(eng));
		for (auto& x : ary)
			x = arrayValue(eng);

		int x = arrayValue(eng), y = arrayValue(eng);
		auto algo = numOfSubarraysWithSameCounts(ary, x, y);
		auto bf = numOfSubarraysWithSameCountsBF(ary, x, y);

		if (algo != bf) {
			for (auto x : ary)
				cout << x << ',';
			cout << "| " << x << ',' << y << '\n';
			cout << "getting " << algo << " instead of " << bf << '\n';
			failed++;
		}
	}

	cout << "failed " << failed << " out of " << total << '\n';
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	//test(10'000'000, 8, 10);

	vector<int> ary = { 3,6,2,6,3,2,3 };
	int x = 3, y = 6;
	
	cout << numOfSubarraysWithSameCountsBF(ary, x, y) << '\n';
	cout << numOfSubarraysWithSameCounts(ary, x, y) << '\n';
}