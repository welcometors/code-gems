#include <vector>
#include <memory>

template <class T>
T maxSubarray(const vector<T> &ary) {
	size_t n = ary.size();

	unique_ptr<T[]> cumulativeSum(new T[n + 1]);
	cumulativeSum[n] = T(0);
	for (size_t i = n; i; i--)
		cumulativeSum[i - 1] = cumulativeSum[i] + ary[i - 1];

	size_t maximumSumSoFarIndex = 0;
	T maximumSumSoFar = cumulativeSum[0];
	T maxSum = ary[0];
	for (size_t i = 1; i < n; i++) {
		if (cumulativeSum[i] > maximumSumSoFar) {
			maximumSumSoFar = cumulativeSum[i];
			maximumSumSoFarIndex = i;
		}
		T sum = cumulativeSum[maximumSumSoFarIndex] - cumulativeSum[i + 1];
		if (sum > maxSum)
			maxSum = sum;
	}
	return maxSum;
}