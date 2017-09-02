// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/largest-sum-contiguous-subarray/

#include <iostream>
#include <vector>
using namespace std;

template<class T>
auto largestSumContinuousSubarray(const vector<T>& ary) {
	if (!ary.size())
		return T(0);

	T maxSoFar = *ary.front(), currentMax = T(0);

	for (auto x: ary) {
		currentMax += x;
		if (maxSoFar < currentMax)
			maxSoFar = currentMax;
		if (currentMax < 0)
			currentMax = 0;
	}

	return maxSoFar;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	vector<int> ary = {-2, -3, 4, -1, -2, 1, 5, -3};
	cout << largestSumContinuousSubarray(ary) << endl;
}