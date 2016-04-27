#include <vector>
using namespace std;

template <class T>
int imerge(vector<T> &ary, int left, int mid, int right, vector<T> &tmp) {
	int l = left, r = mid + 1, i = left, inv = 0;

	while (l <= mid && r <= right) {
		if (ary[l] <= ary[r])
			tmp[i++] = ary[l++];
		else {
			tmp[i++] = ary[r++];
			inv += mid - l + 1;
		}
	}

	while (l <= mid)
		tmp[i++] = ary[l++];

	while (r <= right)
		tmp[i++] = ary[r++];

	std::copy(tmp.begin() + left, tmp.begin() + right + 1, ary.begin() + left);

	return inv;
}

template <class T>
int inversions(vector<T> &ary, int left, int right, vector<T> &tmp) {
	if (right <= left)
		return 0;

	int mid = (left + right) / 2;

	int i = inversions(ary, left, mid, tmp);
	i += inversions(ary, mid + 1, right, tmp);
	i += imerge(ary, left, mid, right, tmp);

	return i;
}