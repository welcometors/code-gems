#include <vector>

template<class T>
size_t lessOrEqualBinarySearch(const vector<T>& t, T n) {
	size_t l = 0, r = t.size() - 1;
	while (l<r) {
		size_t m = (l + r) / 2;
		if (t[m] == n)
			return m;
		if (t[m] < n)
			l = m + 1;
		else
			r = m;
	}
	return t[l] <= n ? l : l - 1;
}