// Author	: Rahul Sharma <rahulsrma26@gmail.com>
// http://www.geeksforgeeks.org/largest-sum-contiguous-subarray/

#include <iostream>
#include <vector>
using namespace std;

template<class Iterator,
	class T = typename std::iterator_traits<Iterator>::value_type,
	typename Comparator = std::less<T>>

auto largestSumContinuousSubarray(Iterator start, Iterator end, Comparator comp = Comparator()) {
	if (start == end)
		return T(0);

	T currentMax = *start, maxSoFar = *start;

	while (++start != end) {
		currentMax = currentMax + *start;
		if (comp(maxSoFar, currentMax))
			maxSoFar = currentMax;
		if (comp(currentMax, T(0)))
			currentMax = T(0);
	}

	return maxSoFar;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(0);

	vector<int> ary = {-2, -3, 4, -1, -2, 1, 5, -3};
	cout << largestSumContinuousSubarray(ary.begin(), ary.end()) << endl;
}