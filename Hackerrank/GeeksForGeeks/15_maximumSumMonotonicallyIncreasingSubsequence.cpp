// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/dynamic-programming-set-14-maximum-sum-increasing-subsequence/
// similar to 04_longestMonotonicallyIncreasingSubsequence

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <memory>
using namespace std;

int maximumSumMonotonicallyIncreasingSubsequence(const vector<int>& ary) {
	if (ary.size() < 1)
		return 0;

	map<int, int> lastWise = { { ary[0], ary[0] } };
	int maxSum = ary[0];

	for (size_t i = 1; i < ary.size(); i++) {
		if (ary[i] < lastWise.begin()->first) {
			lastWise[ary[i]] = ary[i];
		}
		else if (ary[i] > lastWise.begin()->first) {
			auto bestPos = lastWise.lower_bound(ary[i]);
			if (bestPos != lastWise.end()) {
				auto previousElem = bestPos;
				--previousElem;
				int newSum = previousElem->second + ary[i];

				if (bestPos->first != ary[i] || bestPos->second < newSum) {
					stack<int> toBeRemoved;
					for (; bestPos != lastWise.end() && bestPos->second <= newSum; ++bestPos)
						toBeRemoved.push(bestPos->first);
					while (!toBeRemoved.empty()) {
						lastWise.erase(toBeRemoved.top());
						toBeRemoved.pop();
					}

					lastWise[ary[i]] = newSum;
					if (newSum > maxSum)
						maxSum = newSum;
				}
			}
			else {
				maxSum += ary[i];
				lastWise[ary[i]] = maxSum;
			}
		}
	}

	return maxSum;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	vector<int> ary = { 1, 105, 2, 3, 100, 4, 5 };
	cout << maximumSumMonotonicallyIncreasingSubsequence(ary) << '\n';
}