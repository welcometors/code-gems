// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/minimum-number-of-jumps-to-reach-end-of-a-given-array/

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

auto minimumJumpsToEnd(const vector<int>& ary) {
	int n = ary.size();
	if (n < 1)
		return 0;

	if (ary[0] == 0)
		return -1;

	int maxReach = ary[0], step = ary[0], jump = 1;
	for (int i = 1; i < n - 1; i++) {
		maxReach = max(maxReach, i + ary[i]);
		step--;
		if (step == 0) {
			jump++;
			if (maxReach <= i)
				return -1;
			step = maxReach - i;
		}
	}
	return jump;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	const vector<int> ary = { 1, 3, 5, 8, 9, 2, 6, 7, 6, 8, 9 };
	cout << minimumJumpsToEnd(ary) << '\n';
}