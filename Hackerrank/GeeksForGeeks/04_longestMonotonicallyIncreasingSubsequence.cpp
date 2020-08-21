// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/longest-monotonically-increasing-subsequence-size-n-log-n/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

auto longestMonotonicallyIncreasingSubsequence(const vector<int>& ary) {
	vector<vector<int>> sequences = { {ary.front()} };
	vector<int> lastElem = { ary.front() };
	
	for (auto x : ary) {
		int bestIndex = lower_bound(lastElem.begin(), lastElem.end(), x) - lastElem.begin();
		if (bestIndex == 0) {
			sequences[0] = vector<int>{ x };
			lastElem[0] = x;
		}
		else if (bestIndex == sequences.size()) {
			sequences.push_back(sequences.back());
			sequences.back().push_back(x);
			lastElem.push_back(x);
		}
		else if (x != lastElem[bestIndex]) {
			sequences[bestIndex] = sequences[bestIndex - 1];
			sequences[bestIndex].push_back(x);
			lastElem[bestIndex] = x;
		}
	}

	return sequences.back();
}

auto longestMonotonicallyIncreasingSubsequenceLength(const vector<int>& ary) {
	vector<int> lastElem = { ary.front() };

	for (int i = 1; i < ary.size(); i++) {
		int bestIndex = lower_bound(lastElem.begin(), lastElem.end(), ary[i]) - lastElem.begin();
		if (bestIndex == 0)
			lastElem[0] = ary[i];
		else if (bestIndex == lastElem.size())
			lastElem.push_back(ary[i]);
		else
			lastElem[bestIndex] = ary[i];
	}

	return lastElem.size();
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	vector<int> ary = { 0, 8, 4, 12, 2, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };
	auto output = longestMonotonicallyIncreasingSubsequence(ary);
	
	for(auto x: output)
		cout << x << ' ';
	cout << '\n';

	auto length = longestMonotonicallyIncreasingSubsequenceLength(ary);
	cout << "length = " << length << '\n';
}